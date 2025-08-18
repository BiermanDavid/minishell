/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 21:47:26 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/18 13:55:54 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

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
	pids[0] = exec_first_cmd(current, pipes, envp);
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
