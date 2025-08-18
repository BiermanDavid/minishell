/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 00:20:44 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/06 15:32:34 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

int	is_builtin(const char *cmd)
{
	if (!cmd)
		return (0);
	return ((ft_strncmp(cmd, "echo", 5) == 0 && ft_strlen(cmd) == 4)
		|| (ft_strncmp(cmd, "cd", 3) == 0 && ft_strlen(cmd) == 2)
		|| (ft_strncmp(cmd, "pwd", 4) == 0 && ft_strlen(cmd) == 3)
		|| (ft_strncmp(cmd, "env", 4) == 0 && ft_strlen(cmd) == 3)
		|| (ft_strncmp(cmd, "exit", 5) == 0 && ft_strlen(cmd) == 4)
		|| (ft_strncmp(cmd, "export", 7) == 0 && ft_strlen(cmd) == 6)
		|| (ft_strncmp(cmd, "unset", 6) == 0 && ft_strlen(cmd) == 5));
}

int	exec_builtin(t_cmd_node *node, char ***envp)
{
	if (ft_strncmp(node->cmd[0], "echo", 5) == 0
		&& ft_strlen(node->cmd[0]) == 4)
		return (builtin_echo(node->cmd));
	else if (ft_strncmp(node->cmd[0], "cd", 3) == 0
		&& ft_strlen(node->cmd[0]) == 2)
		return (builtin_cd(node->cmd, envp));
	else if (ft_strncmp(node->cmd[0], "pwd", 4) == 0
		&& ft_strlen(node->cmd[0]) == 3)
		return (builtin_pwd());
	else if (ft_strncmp(node->cmd[0], "env", 4) == 0
		&& ft_strlen(node->cmd[0]) == 3)
		return (builtin_env(*envp));
	else if (ft_strncmp(node->cmd[0], "exit", 5) == 0
		&& ft_strlen(node->cmd[0]) == 4)
		return (builtin_exit(node->cmd));
	else if (ft_strncmp(node->cmd[0], "export", 7) == 0
		&& ft_strlen(node->cmd[0]) == 6)
		return (builtin_export(node->cmd, envp));
	else if (ft_strncmp(node->cmd[0], "unset", 6) == 0
		&& ft_strlen(node->cmd[0]) == 5)
		return (builtin_unset(node->cmd, envp));
	return (1);
}
