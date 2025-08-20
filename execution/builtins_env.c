/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 23:45:17 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/19 22:36:39 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

/**
 * Handles export command when the argument contains a value assignment.
 * Validates identifier and sets the environment variable.
 * Returns 0 on success, 1 on failure.
 */
static int	handle_export_with_value(char *arg, char *eq, char ***envp)
{
	int			result;
	extern int	g_exit_status;

	*eq = '\0';
	if (!is_valid_identifier(arg))
	{
		*eq = '=';
		write(STDERR_FILENO, "minishell: export: `", 20);
		write(STDERR_FILENO, arg, ft_strlen(arg));
		write(STDERR_FILENO, "': not a valid identifier\n", 26);
		g_exit_status = 1;
		return (1);
	}
	result = env_set(envp, arg, eq + 1);
	*eq = '=';
	return (result);
}

/**
 * Processes a single argument for the export command.
 * Determines if it's a value assignment or declaration only.
 * Returns 0 on success, 1 on failure.
 */
static int	handle_export_argument(char *arg, char ***envp)
{
	char		*eq;
	extern int	g_exit_status;

	eq = ft_strchr((char *)arg, '=');
	if (eq)
		return (handle_export_with_value(arg, eq, envp));
	else if (!is_valid_identifier(arg))
	{
		write(STDERR_FILENO, "minishell: export: `", 20);
		write(STDERR_FILENO, arg, ft_strlen(arg));
		write(STDERR_FILENO, "': not a valid identifier\n", 26);
		g_exit_status = 1;
		return (1);
	}
	return (0);
}

/**
 * Implements the export builtin command functionality.
 * Exports variables to environment or prints all exported variables.
 * Returns 0 on success.
 */
int	builtin_export(char **args, char ***envp)
{
	int	i;
	int	error;

	if (!args[1])
	{
		env_print(*envp);
		return (0);
	}
	i = 1;
	error = 0;
	while (args[i])
	{
		if (handle_export_argument(args[i], envp))
			error = 1;
		i++;
	}
	return (error);
}

/**
 * Implements the unset builtin command functionality.
 * Removes specified environment variables.
 * Returns 0 on success.
 */
int	builtin_unset(char **args, char ***envp)
{
	int			i;
	int			error;
	extern int	g_exit_status;

	i = 1;
	error = 0;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			write(STDERR_FILENO, "minishell: unset: `", 19);
			write(STDERR_FILENO, args[i], ft_strlen(args[i]));
			write(STDERR_FILENO, "': not a valid identifier\n", 26);
			g_exit_status = 1;
			error = 1;
		}
		else
			env_unset(envp, args[i]);
		i++;
	}
	return (error);
}

/**
 * Implements the env builtin command functionality.
 * Prints all environment variables to stdout.
 * Returns 0 on success.
 */
int	builtin_env(char **env)
{
	env_print(env);
	return (0);
}
