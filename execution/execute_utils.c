/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 10:00:00 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/17 21:16:25 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

/**
 * Frees a NULL-terminated array of strings.
 * Safely handles NULL input.
 */
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

/**
 * Joins directory path and command name with '/' separator.
 * Returns allocated string or NULL on failure.
 */
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

/**
 * Extracts PATH directories from environment.
 * Returns array of directory paths split by ':' or NULL if PATH not found.
 */
char	**get_path_dirs(char **env)
{
	char	*path_env;

	path_env = env_get(env, "PATH");
	if (path_env)
		return (ft_split(path_env, ':'));
	return (NULL);
}

/**
 * Executes command with absolute or relative path.
 * Checks file existence and permissions before execution.
 */
void	exec_absolute_path(t_cmd_node *node, char **env)
{
	struct stat	statbuf;

	if (access(node->cmd[0], F_OK) != 0)
		exit(print_command_not_found(node->cmd[0]));
	if (stat(node->cmd[0], &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
	{
		write(STDERR_FILENO, "minishell: ", 11);
		write(STDERR_FILENO, node->cmd[0], ft_strlen(node->cmd[0]));
		write(STDERR_FILENO, ": is a directory\n", 17);
		exit(126);
	}
	if (access(node->cmd[0], X_OK) != 0)
		exit(print_permission_denied(node->cmd[0]));
	execve(node->cmd[0], node->cmd, env);
	exit(print_permission_denied(node->cmd[0]));
}

/**
 * Searches PATH directories for executable command.
 * Tries each directory until command is found and executed.
 */
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
			{
				execve(cmd_path, node->cmd, env);
				free(cmd_path);
				exit(print_permission_denied(node->cmd[0]));
			}
			free(cmd_path);
			exit(print_permission_denied(node->cmd[0]));
		}
		if (cmd_path)
			free(cmd_path);
		i++;
	}
	exit(print_command_not_found(node->cmd[0]));
}
