/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 20:24:05 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/19 22:11:21 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "parse.h"

int	g_exit_status = 0;

/**
 * Executes a single command node with proper expansion and redirection.
 * Processing order:
 * 1. Validates node has command
 * 2. Expands wildcards and variables in arguments
 * 3. Checks for standalone variable assignment (VAR=value)
 * 4. Executes builtin commands with redirection support
 * 5. Executes external commands via PATH lookup
 * Returns command exit status (0 = success, non-zero = error).
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
