/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 10:00:00 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/04 22:41:40 by dgessner         ###   ########.fr       */
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
	execve(node->cmd[0], node->cmd, env);
}

void	try_path_execution(t_cmd_node *node, char **env, char **paths)
{
	char	*cmd_path;
	int		i;

	i = 0;
	while (paths && paths[i])
	{
		cmd_path = join_path(paths[i], node->cmd[0]);
		if (cmd_path)
		{
			execve(cmd_path, node->cmd, env);
			free(cmd_path);
		}
		i++;
	}
}
