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
 * Closes unused pipe file descriptors for the current process.
 */
static void	close_unused_pipes(t_pipe_info *pipe_info, int in_fd, int out_fd)
{
	int	i;

	i = 0;
	while (i < pipe_info->total)
	{
		if (pipe_info->pipes[i][0] != in_fd)
			close(pipe_info->pipes[i][0]);
		if (pipe_info->pipes[i][1] != out_fd)
			close(pipe_info->pipes[i][1]);
		i++;
	}
}

/**
 * Spawns a process for a single pipeline stage.
 */
pid_t	spawn_stage(t_cmd_node *node, int fds[2],
		t_pipe_info *pipe_info, char ***envp)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		setup_child_signals();
		dup2(fds[0], STDIN_FILENO);
		dup2(fds[1], STDOUT_FILENO);
		close_unused_pipes(pipe_info, fds[0], fds[1]);
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
	t_pipe_info *pipe_info, char ***envp)
{
	int	i;
	int	in_fd;
	int	fds[2];

	i = 0;
	while (node && node->cmd_type == CMD_PIPE)
	{
		pipe(pipe_info->pipes[i]);
		if (i == 0)
			in_fd = STDIN_FILENO;
		else
			in_fd = pipe_info->pipes[i - 1][0];
		pipe_info->total = i + 1;
		fds[0] = in_fd;
		fds[1] = pipe_info->pipes[i][1];
		pids[i] = spawn_stage(node, fds, pipe_info, envp);
		if (i > 0)
			close(pipe_info->pipes[i - 1][0]);
		close(pipe_info->pipes[i][1]);
		node = node->next;
		i++;
	}
	return (i);
}

void	final_stage(t_cmd_node *node, pid_t *pids,
	t_pipe_info *pipe_info, char ***envp)
{
	int	in_fd;
	int	i;
	int	fds[2];

	i = pipe_info->total;
	if (i == 0)
		in_fd = STDIN_FILENO;
	else
		in_fd = pipe_info->pipes[i - 1][0];
	pipe_info->total = i + 1;
	fds[0] = in_fd;
	fds[1] = STDOUT_FILENO;
	pids[i] = spawn_stage(node, fds, pipe_info, envp);
	if (i > 0)
		close(pipe_info->pipes[i - 1][0]);
}

/**
 * Executes a pipeline of commands.
 */
t_cmd_node	*exec_pipeline(t_cmd_node *start, char ***envp)
{
	t_cmd_node	*node;
	pid_t		pids[64];
	int			status;
	int			count;
	t_pipe_info	pipe_info;

	node = start;
	count = setup_pipeline(start, pids, &pipe_info, envp);
	final_stage(node, pids, &pipe_info, envp);
	while (count >= 0)
	{
		waitpid(pids[count], &status, 0);
		count--;
	}
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else
		g_exit_status = 1;
	return (node->next);
}
