/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 19:51:40 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/20 05:53:40 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

/**
 * Counts commands in pipeline sequence including final non-pipe command.
 * Iterates through linked list until non-pipe command found.
 * @param start First command node in pipeline sequence
 * @return Total command count in pipeline (minimum 1)
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
 * Creates pipe file descriptors for inter-process communication.
 * Allocates read/write pipe pairs for each command connection in pipeline. 
 * @param pipes Array to store pipe file descriptor pairs
 * @param pipe_count Number of pipes to create (cmd_count - 1)
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
 * Executes pipeline of commands with inter-process pipes and signals.
 * Sets up pipes, manages signal handling, delegates to process execution,
 * then returns next non-pipeline command node.
 * @param start First command node in pipeline sequence
 * @param envp Pointer to environment variables array (modifiable)
 * @return Pointer to next command node after pipeline ends
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

/**
 * Closes all pipe file descriptors in parent process for cleanup.
 * Prevents hanging pipes by closing unused file descriptors,
 * essential for proper pipeline termination and EOF signaling.
 * @param pipes Array of pipe file descriptor pairs
 * @param pipe_count Number of pipes to close
 */
void	close_all_pipes(int pipes[][2], int pipe_count)
{
	int	i;

	i = 0;
	while (i < pipe_count)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}
