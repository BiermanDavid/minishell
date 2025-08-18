/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 15:33:17 by dabierma          #+#    #+#             */
/*   Updated: 2025/08/07 02:10:57 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

/**
 * Executes first command in pipeline.
 * Writes output to pipe, reads from stdin.
 */
pid_t	exec_first_cmd(t_cmd_node *node, int pipes[][2], char ***envp)
{
	pid_t	pid;
	int		j;

	pid = fork();
	if (pid == 0)
	{
		setup_child_signals();
		dup2(pipes[0][1], STDOUT_FILENO);
		j = 0;
		while (j < 10)
		{
			close(pipes[j][0]);
			close(pipes[j][1]);
			j++;
		}
		if (apply_redirections(node->files, *envp) == -1)
			exit(1);
		if (is_builtin(node->cmd[0]))
			exit(exec_builtin(node, envp));
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
	int		j;

	pid = fork();
	if (pid == 0)
	{
		setup_child_signals();
		dup2(pipes[pipe_count - 1][0], STDIN_FILENO);
		j = 0;
		while (j < pipe_count)
		{
			close(pipes[j][0]);
			close(pipes[j][1]);
			j++;
		}
		if (apply_redirections(node->files, *envp) == -1)
			exit(1);
		if (is_builtin(node->cmd[0]))
			exit(exec_builtin(node, envp));
		exec_command(node, *envp);
		exit(127);
	}
	return (pid);
}

/**
 * Sets up pipe connections for middle command.
 * Connects input and output pipes appropriately.
 */
static void	setup_middle_cmd_pipes(int pipes[][2], int i)
{
	int	pipe_count;
	int	j;

	dup2(pipes[i - 1][0], STDIN_FILENO);
	dup2(pipes[i][1], STDOUT_FILENO);
	pipe_count = i + 1;
	j = 0;
	while (j < pipe_count)
	{
		close(pipes[j][0]);
		close(pipes[j][1]);
		j++;
	}
}

/**
 * Executes middle command in pipeline.
 * Reads from previous pipe, writes to next pipe.
 */
pid_t	exec_middle_cmd(t_cmd_node *node, int pipes[][2], int i, char ***envp)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		setup_child_signals();
		setup_middle_cmd_pipes(pipes, i);
		if (apply_redirections(node->files, *envp) == -1)
			exit(1);
		if (is_builtin(node->cmd[0]))
			exit(exec_builtin(node, envp));
		exec_command(node, *envp);
		exit(127);
	}
	return (pid);
}

/**
 * Closes all pipe file descriptors in parent process.
 * Essential for proper pipeline termination.
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
