/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 10:00:00 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/06 18:20:56 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

void	free_split(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

char	*join_path(const char *dir, const char *cmd)
{
	char	*tmp;
	char	*path;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	path = ft_strjoin(tmp, cmd);
	free(tmp);
	return (path);
}

char	**get_path_dirs(char **env)
{
	char	*path_env;

	path_env = env_get(env, "PATH");
	if (path_env)
		return (ft_split(path_env, ':'));
	return (NULL);
}

void	exec_absolute_path(t_cmd_node *node, char **env)
{
	if (access(node->cmd[0], F_OK) != 0)
		print_command_not_found(node->cmd[0]);
	if (access(node->cmd[0], X_OK) != 0)
		print_permission_denied(node->cmd[0]);
	execve(node->cmd[0], node->cmd, env);
	print_permission_denied(node->cmd[0]);
}

void	try_path_execution(t_cmd_node *node, char **env, char **paths)
{
	char	*cmd_path;
	int		i;

	i = 0;
	while (paths && paths[i])
	{
		cmd_path = join_path(paths[i], node->cmd[0]);
		if (cmd_path && access(cmd_path, F_OK) == 0)
		{
			if (access(cmd_path, X_OK) == 0)
				execve(cmd_path, node->cmd, env);
			free(cmd_path);
			print_permission_denied(node->cmd[0]);
		}
		if (cmd_path)
			free(cmd_path);
		i++;
	}
	print_command_not_found(node->cmd[0]);
}
