/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 23:45:17 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/03 02:07:03 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include <ctype.h>

int	is_valid_identifier(const char *str)
{
	if (!str || (!isalpha(str[0]) && str[0] != '_'))
		return (0);
	for (int i = 1; str[i] && str[i] != '='; i++)
	{
		if (!isalnum(str[i]) && str[i] != '_')
			return (0);
	}
	return (1);
}

int is_assignment(const char *str)
{
	int i = 0;
	char *eq = strchr(str, '=');
	if (!eq || eq == str)
		return (0);
	if (!isalpha(str[0]) && str[0] != '_')
		return (0);
	while (str + i < eq)
	{
		if (!isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}


int	builtin_export(char **args, char ***envp)
{
	int		i;
	char	*eq;

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
			if (!is_valid_identifier(args[i]))
			{
				fprintf(stderr, "minishell: export: `%s': not a valid identifier\n", args[i]);
				*eq = '=';
				i++;
				continue ;
			}
			env_set(envp, args[i], eq + 1);
			*eq = '=';
		}
		else
		{
			if (!is_valid_identifier(args[i]))
				fprintf(stderr, "minishell: export: `%s': not a valid identifier\n", args[i]);
		}
		i++;
	}
	return (0);
}

int	builtin_unset(char **args, char ***envp)
{
	int	i;

	i = 1;
	while (args[i])
	{
		env_unset(envp, args[i]);
		i++;
	}
	return (0);
}

int	builtin_cd(char **args, char **env)
{
	char	*path;

	path = args[1];
	if (!path)
		path = env_get(env, "HOME");
	if (!path || chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

int	builtin_env(char **env)
{
	env_print(env);
	return (0);
}
