/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 19:51:40 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/07 02:09:59 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

/**
 * Counts commands in pipeline.
 * Returns number of commands in pipeline.
 */
int	count_pipeline_commands(t_cmd_node *start)
{
	t_cmd_node	*current;
	int			cmd_count;

	current = start;
	cmd_count = 0;
	while (current && current->cmd_type == CMD_PIPE)
	{
		current = current->next;
		cmd_count++;
	}
	cmd_count++;
	return (cmd_count);
}

/**
 * Creates pipes for pipeline.
 * Sets up all necessary pipes for command chain.
 */
void	setup_pipes(int (*pipes)[2], int pipe_count)
{
	int	i;

	i = 0;
	while (i < pipe_count)
	{
		pipe(pipes[i]);
		i++;
	}
}

/**
 * Executes all middle commands in pipeline.
 * Handles commands between first and last.
 */
void	exec_middle_commands(t_cmd_node *start, pid_t *pids, int pipes[][2],
		char ***envp)
{
	t_cmd_node	*current;
	int			i;
	int			cmd_count;
	t_cmd_node	*temp;

	current = start->next;
	i = 1;
	cmd_count = 0;
	temp = start;
	while (temp && temp->cmd_type == CMD_PIPE)
	{
		temp = temp->next;
		cmd_count++;
	}
	cmd_count++;
	while (i < cmd_count - 1)
	{
		pids[i] = exec_middle_cmd(current, pipes, i, envp);
		current = current->next;
		i++;
	}
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
 * Helper function to execute pipeline processes.
 * Splits exec_pipeline to comply with Norm variable limit.
 */
static t_cmd_node	*execute_pipeline_processes(t_cmd_node *start,
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
	current = start;
	pids[0] = exec_first_cmd(current, pipes, envp);
	exec_middle_commands(start, pids, pipes, envp);
	while (current->next)
		current = current->next;
	pids[cmd_count - 1] = exec_last_cmd(current, pipes, pipe_count, envp);
	close_all_pipes(pipes, pipe_count);
	wait_for_pipeline(pids, cmd_count);
	free(pids);
	free(pipes);
	return (current->next);
}

/**
 * Executes a complete pipeline of commands.
 * Creates pipes and manages all processes with minimal signal handling.
 */
t_cmd_node	*exec_pipeline(t_cmd_node *start, char ***envp)
{
	int			cmd_count;
	int			pipe_count;
	t_cmd_node	*result;

	set_minimal_signals();
	cmd_count = count_pipeline_commands(start);
	pipe_count = cmd_count - 1;
	result = execute_pipeline_processes(start, cmd_count, pipe_count, envp);
	initialize_shell_signals();
	return (result);
}
