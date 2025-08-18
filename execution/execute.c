/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 20:24:05 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/18 17:56:42 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "parse.h"

int	g_exit_status = 0;

/**
 * Executes a single command node with proper expansion and redirection.
 * Handles variable assignments, builtins, and external commands.
 */
int	execute_single(t_cmd_node *node, char ***envp)
{
	if (!node->cmd || !node->cmd[0])
		return (0);
	expand_command_args(node, *envp);
	if (!node->cmd[0] || !node->cmd[0][0])
		return (0);
	if (is_variable_assignment(node))
		return (handle_assignment(node, envp));
	if (is_builtin(node->cmd[0]))
		return (exec_builtin_redir(node, envp));
	return (exec_external(node, *envp));
}

/**
 * Main execution manager that processes command list sequentially.
 * Handles pipelines and regular commands with proper exit status tracking.
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
		{
			node = exec_pipeline(node, envp);
		}
		else
		{
			result = execute_single(node, envp);
			g_exit_status = result;
			node = node->next;
		}
	}
	return (g_exit_status);
}

/**
 * Checks if a command node represents a variable assignment.
 * Returns 1 if it's an assignment with no additional arguments.
 */
int	is_variable_assignment(t_cmd_node *node)
{
	return (is_assignment(node->cmd[0]) && node->cmd[1] == NULL);
}
