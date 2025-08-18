/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 19:51:40 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/18 13:55:17 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

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
