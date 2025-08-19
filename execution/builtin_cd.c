/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 10:15:06 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/19 20:41:19 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

/**
 * Determines the target directory for the cd command based on arguments.
 * Returns HOME directory if no argument provided, OLDPWD if "-" is given,
 * or the specified directory path.
 */
static char	*get_cd_target(char **args, char **envp)
{
	char	*target;

	if (!args[1])
		return (env_get(envp, "HOME"));
	if (ft_strncmp(args[1], "-", 2) == 0 && ft_strlen(args[1]) == 1)
	{
		target = env_get(envp, "OLDPWD");
		if (!target)
		{
			write(STDOUT_FILENO, "cd: OLDPWD not set\n", 19);
			return (NULL);
		}
		write(STDOUT_FILENO, target, ft_strlen(target));
		write(STDOUT_FILENO, "\n", 1);
		return (target);
	}
	return (args[1]);
}

/**
 * Implements the cd builtin command functionality.
 * Changes directory and updates PWD and OLDPWD environment variables.
 * Returns 0 on success, 1 on failure.
 */
int	builtin_cd(char **args, char ***envp)
{
	char	cwd[PATH_MAX];
	char	*target;

	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("cd: getcwd failed");
		return (1);
	}
	target = get_cd_target(args, *envp);
	if (!target)
		return (1);
	if (chdir(target) != 0)
	{
		print_directory_not_found(target);
		return (1);
	}
	env_set(envp, "OLDPWD", cwd);
	if (getcwd(cwd, sizeof(cwd)))
		env_set(envp, "PWD", cwd);
	return (0);
}
