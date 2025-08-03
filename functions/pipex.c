/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 19:51:40 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/03 03:39:27 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

/**
 * Spawns a process for a single pipeline stage.
 */
pid_t	spawn_stage(t_cmd_node *node, int in_fd, int out_fd,
		int pipes[64][2], int total, char ***envp)
{
	pid_t	pid;
	int		i;

	pid = fork();
	if (pid == 0)
	{
		setup_child_signals();
		dup2(in_fd, STDIN_FILENO);
		dup2(out_fd, STDOUT_FILENO);
		i = 0;
		while (i < total)
		{
			if (pipes[i][0] != in_fd)
				close(pipes[i][0]);
			if (pipes[i][1] != out_fd)
				close(pipes[i][1]);
			i++;
		}
		if (apply_redirections(node->files) == -1)
			exit(1);
		if (is_builtin(node->cmd[0]))
			exit(exec_builtin(node, envp));
		exec_command(node, *envp);
		perror(node->cmd[0]);
		exit(127);
	}
	return (pid);
}

int	setup_pipeline(t_cmd_node *node, pid_t *pids,
	int pipes[64][2], char ***envp)
{
	int	i;
	int	in_fd;

	i = 0;
	while (node && node->cmd_type == CMD_PIPE)
	{
		pipe(pipes[i]);
		if (i == 0)
			in_fd = STDIN_FILENO;
		else
			in_fd = pipes[i - 1][0];
		pids[i] = spawn_stage(node, in_fd,
				pipes[i][1], pipes, i + 1, envp);
		if (i > 0)
			close(pipes[i - 1][0]);
		close(pipes[i][1]);
		node = node->next;
		i++;
	}
	return (i);
}

void	final_stage(t_cmd_node *node, pid_t *pids,
	int pipes[64][2], int i, char ***envp)
{
	int	in_fd;

	if (i == 0)
		in_fd = STDIN_FILENO;
	else
		in_fd = pipes[i - 1][0];
	pids[i] = spawn_stage(node, in_fd, STDOUT_FILENO,
			pipes, i + 1, envp);
	if (i > 0)
		close(pipes[i - 1][0]);
}

void	wait_for_pipeline(pid_t *pids, int count, int *status)
{
	while (count >= 0)
	{
		waitpid(pids[count], status, 0);
		count--;
	}
}

/**
 * Executes a pipeline of commands.
 */
t_cmd_node	*exec_pipeline(t_cmd_node *start, char ***envp)
{
	t_cmd_node	*node;
	pid_t		pids[64];
	int			pipes[64][2];
	int			status;
	int			count;

	node = start;
	count = setup_pipeline(start, pids, pipes, envp);
	final_stage(node, pids, pipes, count, envp);
	wait_for_pipeline(pids, count, &status);
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else
		g_exit_status = 1;
	return (node->next);
}