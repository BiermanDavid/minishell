/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 15:33:17 by dabierma          #+#    #+#             */
/*   Updated: 2025/08/20 00:56:09 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

/**
 * Executes first command in pipeline.
 * Writes output to pipe, reads from stdin.
 */
pid_t	exec_first_cmd(t_cmd_node *node, int pipes[][2], int pipe_count,
			char ***envp)
{
	pid_t	pid;
	int		result;

	pid = fork();
	if (pid == 0)
	{
		setup_child_signals();
		dup2(pipes[0][1], STDOUT_FILENO);
		close_all_pipes(pipes, pipe_count);
		if (apply_redirections(node->files, *envp) == -1)
			exit(1);
		if (is_builtin(node->cmd[0]))
		{
			result = exec_builtin(node, envp);
			if (result == 130)
				exit(0);
			exit(result);
		}
		exec_command(node, *envp);
		exit(127);
	}
	return (pid);
}

/**
 * Executes last command in pipeline.
 * Reads from pipe, writes to stdout.
 */
pid_t	exec_last_cmd(t_cmd_node *node, int pipes[][2], int pipe_count,
		char ***envp)
{
	pid_t	pid;
	int		result;

	pid = fork();
	if (pid == 0)
	{
		setup_child_signals();
		dup2(pipes[pipe_count - 1][0], STDIN_FILENO);
		close_all_pipes(pipes, pipe_count);
		if (apply_redirections(node->files, *envp) == -1)
			exit(1);
		if (is_builtin(node->cmd[0]))
		{
			result = exec_builtin(node, envp);
			if (result == 130)
				exit(0);
			exit(result);
		}
		exec_command(node, *envp);
		exit(127);
	}
	return (pid);
}

/**
 * Waits for all pipeline processes to complete.
 * Sets global exit status from last command.
 */
void	wait_for_pipeline(pid_t *pids, int cmd_count)
{
	int	status;
	int	i;

	i = 0;
	while (i < cmd_count)
	{
		waitpid(pids[i], &status, 0);
		if (i == cmd_count - 1)
		{
			if (WIFEXITED(status))
				g_exit_status = WEXITSTATUS(status);
			else
				g_exit_status = 1;
		}
		i++;
	}
}

/**
 * Sets up and executes all pipeline processes.
 * Handles first, middle, and last command execution.
 */
static void	setup_pipeline_processes(t_cmd_node *start, pid_t *pids,
	int pipes[][2], char ***envp)
{
	t_cmd_node	*current;
	int			cmd_count;

	current = start;
	cmd_count = count_pipeline_commands(start);
	pids[0] = exec_first_cmd(current, pipes, cmd_count - 1, envp);
	exec_middle_commands(start, pids, pipes, envp);
	while (current->next)
		current = current->next;
	pids[cmd_count - 1] = exec_last_cmd(current, pipes, cmd_count - 1, envp);
}

/**
 * Helper function to execute pipeline processes.
 * Splits exec_pipeline to comply with Norm variable limit.
 */
t_cmd_node	*execute_pipeline_processes(t_cmd_node *start,
		int cmd_count, int pipe_count, char ***envp)
{
	t_cmd_node	*current;
	pid_t		*pids;
	int			(*pipes)[2];

	if (cmd_count > 1000)
		return (start->next);
	pids = malloc(cmd_count * sizeof(pid_t));
	pipes = malloc(pipe_count * sizeof(int [2]));
	if (!pids || !pipes)
	{
		free(pids);
		free(pipes);
		return (start->next);
	}
	setup_pipes(pipes, pipe_count);
	setup_pipeline_processes(start, pids, pipes, envp);
	close_all_pipes(pipes, pipe_count);
	wait_for_pipeline(pids, cmd_count);
	current = start;
	while (current->next)
		current = current->next;
	free(pids);
	free(pipes);
	return (current->next);
}
