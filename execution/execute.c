/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 20:24:05 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/20 05:27:13 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "parse.h"

int	g_exit_status = 0;

/**
 * Executes single command with expansion, assignments, and redirections.
 * Validates input, expands variables/wildcards, handles assignments,
 * then routes to builtin or external command execution.
 * @param node Command node with command array and redirection list
 * @param envp Pointer to environment variables array (modifiable)
 * @return Command exit status (0=success, 1-255=errors, 127=not found)
 */
int	execute_single(t_cmd_node *node, char ***envp)
{
	if (!node->cmd || !node->cmd[0])
		return (0);
	expand_command_args(node, *envp);
	if (!node->cmd[0] || !node->cmd[0][0])
		return (0);
	if (is_assignment(node->cmd[0]) && node->cmd[1] == NULL)
		return (handle_assignment(node, envp));
	if (is_builtin(node->cmd[0]))
		return (exec_builtin_redir(node, envp));
	return (exec_external(node, *envp));
}

/**
 * Core execution that processes parsed command list 
 * and manages shell exit status.
 * Iterates through command nodes, detecting pipeline vs single commands.
 * For pipelines, delegates to exec_pipeline() which returns next non-pipeline
 * node. For single commands, executes via execute_single() 
 * and updates global exit status. Handles SIGINT (Ctrl+C) 
 * by returning -130 to signal interruption.
 * @param cmd_list Parsed command list containing command nodes to execute
 * @param envp Pointer to environment variables array (modifiable)
 * @return Global exit status of last executed command, -130 on interruption,
 * -1 on invalid input
 */
int	execution_manager(t_cmd_list *cmd_list, char ***envp)
{
	t_cmd_node	*node;
	int			result;

	if (!cmd_list || !cmd_list->head)
		return (-1);
	node = cmd_list->head;
	while (node)
	{
		if (node->cmd_type == CMD_PIPE)
			node = exec_pipeline(node, envp);
		else
		{
			result = execute_single(node, envp);
			if (result == 130)
				return (-130);
			g_exit_status = result;
			node = node->next;
		}
	}
	return (g_exit_status);
}
