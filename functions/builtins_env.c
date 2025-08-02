/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 23:45:17 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/03 00:26:41 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

int builtin_export(char **args, char ***envp)
{
	int i;
	char *eq;

	if (!args[1])
	{
		env_print(*envp);
		return (0);
	}
	i = 1;
	while (args[i])
	{
		eq = strchr(args[i], '=');
		if (eq)
		{
			*eq = '\0';
			env_set(envp, args[i], eq + 1);
			*eq = '=';
		}
		i++;
	}
	return (0);
}

int builtin_unset(char **args, char ***envp)
{
	int i = 1;

	while (args[i])
	{
		env_unset(envp, args[i]);
		i++;
	}
	return (0);
}

int builtin_cd(char **args, char **env)
{
	char *path = args[1];

	if (!path)
		path = env_get(env, "HOME");
	if (!path || chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

int builtin_env(char **env)
{
	env_print(env);
	return (0);
}
