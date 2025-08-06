/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 20:24:05 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/03 10:20:01 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "parse.h"

int	g_exit_status = 0;

int	execute_single(t_cmd_node *node, char ***envp)
{
	if (!node->cmd || !node->cmd[0])
		return (0);
	if (is_variable_assignment(node))
		return (handle_assignment(node, envp));
	if (is_builtin(node->cmd[0]))
		return (exec_builtin_redir(node, envp));
	return (exec_external(node, *envp));
}

int	execution_manager(t_cmd_list *cmd_list, char ***envp)
{
	t_cmd_node	*node;

	if (!cmd_list || !cmd_list->head)
		return (-1);
	node = cmd_list->head;
	while (node)
	{
		if (node->cmd_type == CMD_PIPE)
			node = exec_pipeline(node, envp);
		else
		{
			g_exit_status = execute_single(node, envp);
			node = node->next;
		}
	}
	return (0);
}
