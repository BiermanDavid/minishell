/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 00:20:44 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/03 00:30:11 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

int	is_builtin(const char *cmd)
{
	if (!cmd)
		return (0);
	return (strcmp(cmd, "echo") == 0 || strcmp(cmd, "cd") == 0
		|| strcmp(cmd, "pwd") == 0 || strcmp(cmd, "env") == 0
		|| strcmp(cmd, "exit") == 0 || strcmp(cmd, "export") == 0
		|| strcmp(cmd, "unset") == 0);
}

int	exec_builtin(t_cmd_node *node, char ***envp)
{
	if (strcmp(node->cmd[0], "echo") == 0)
		return (builtin_echo(node->cmd));
	else if (strcmp(node->cmd[0], "cd") == 0)
		return (builtin_cd(node->cmd, *envp));
	else if (strcmp(node->cmd[0], "pwd") == 0)
		return (builtin_pwd());
	else if (strcmp(node->cmd[0], "env") == 0)
		return (builtin_env(*envp));
	else if (strcmp(node->cmd[0], "exit") == 0)
		return (builtin_exit(node->cmd));
	else if (strcmp(node->cmd[0], "export") == 0)
		return (builtin_export(node->cmd, envp));
	else if (strcmp(node->cmd[0], "unset") == 0)
		return (builtin_unset(node->cmd, envp));
	return (1);
}
