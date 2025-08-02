/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 19:51:40 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/02 22:32:35 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

/**
 * Pipeline Execution
 */
pid_t	spawn_stage(t_cmd_node *node, int in_fd, int out_fd, int pipes[64][2], int total)
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
			exit(exec_builtin(node));
		execvp(node->cmd[0], node->cmd);
		perror(node->cmd[0]);
		exit(127);
	}
	return (pid);
}

t_cmd_node	*exec_pipeline(t_cmd_node *start)
{
	t_cmd_node	*node;
	pid_t		pids[64];
	int			pipes[64][2];
	int			i;
	int			status;

	node = start;
	i = 0;
	while (node && node->cmd_type == CMD_PIPE)
	{
		pipe(pipes[i]);
		pids[i] = spawn_stage(node,
				i == 0 ? STDIN_FILENO : pipes[i - 1][0],
				pipes[i][1], pipes, i + 1);
		if (i > 0)
			close(pipes[i - 1][0]);
		close(pipes[i][1]);
		node = node->next;
		i++;
	}
	pids[i] = spawn_stage(node,
			i == 0 ? STDIN_FILENO : pipes[i - 1][0],
			STDOUT_FILENO, pipes, i + 1);
	if (i > 0)
		close(pipes[i - 1][0]);
	while (i >= 0)
	{
		waitpid(pids[i], &status, 0);
		i--;
	}
	g_exit_status = WIFEXITED(status) ? WEXITSTATUS(status) : 1;
	return (node->next);
}
