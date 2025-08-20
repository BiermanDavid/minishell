/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 21:47:26 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/20 06:01:49 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

/**
 * Executes all middle commands in pipeline with dual pipe redirection.
 * Iterates through pipeline, forking processes for commands between
 * first and last, with stdin from previous pipe and stdout to next pipe.
 * @param start First command node in pipeline sequence
 * @param pids Array to store process IDs for each command
 * @param pipes Array of pipe file descriptor pairs
 * @param envp Pointer to environment variables array (modifiable)
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
 * Executes single middle command with dual pipe redirection.
 * Forks process, redirects stdin from previous pipe and stdout to next pipe,
 * applies command redirections, then executes builtin or external command.
 * @param node Command node to execute
 * @param pipes Array of pipe file descriptor pairs
 * @param i Command index in pipeline (determines pipe connections)
 * @param envp Pointer to environment variables array (modifiable)
 * @return Process ID of forked child process
 */
pid_t	exec_middle_cmd(t_cmd_node *node, int pipes[][2], int i, char ***envp)
{
	pid_t	pid;
	int		result;

	pid = fork();
	if (pid == 0)
	{
		setup_child_signals();
		dup2(pipes[i - 1][0], STDIN_FILENO);
		dup2(pipes[i][1], STDOUT_FILENO);
		close_all_pipes(pipes, i + 1);
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
