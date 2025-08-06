/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 23:45:17 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/04 21:50:04 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

static int	handle_export_argument(char *arg, char ***envp)
{
	char	*eq;
	int		result;

	eq = ft_strchr((char *)arg, '=');
	if (eq)
	{
		*eq = '\0';
		if (!is_valid_identifier(arg))
		{
			printf("minishell: export: `%s': not a valid identifier\n", arg);
			*eq = '=';
			return (1);
		}
		result = env_set(envp, arg, eq + 1);
		*eq = '=';
		return (result);
	}
	else if (!is_valid_identifier(arg))
	{
		printf("minishell: export: `%s': not a valid identifier\n", arg);
		return (1);
	}
	return (0);
}

int	builtin_export(char **args, char ***envp)
{
	int	i;

	if (!args[1])
	{
		env_print(*envp);
		return (0);
	}
	i = 1;
	while (args[i])
	{
		handle_export_argument(args[i], envp);
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

int	builtin_env(char **env)
{
	env_print(env);
	return (0);
}
