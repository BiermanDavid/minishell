/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 21:12:35 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/20 00:48:08 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

/**
 * Implements the echo builtin command functionality.
 * Prints arguments to stdout with optional newline suppression (-n flag).
 * Returns 0 on success.
 */
int	builtin_echo(char **args)
{
	int	i;
	int	newline;

	newline = 1;
	i = 1;
	if (args[1] && ft_strncmp(args[1], "-n", 3) == 0 && ft_strlen(args[1]) == 2)
	{
		newline = 0;
		i = 2;
	}
	while (args[i])
	{
		write(STDOUT_FILENO, args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (newline)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}

/**
 * Implements the pwd builtin command functionality.
 * Prints the current working directory to stdout.
 * Returns 0 on success.
 */
int	builtin_pwd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)))
	{
		write(STDOUT_FILENO, cwd, ft_strlen(cwd));
		write(STDOUT_FILENO, "\n", 1);
	}
	return (0);
}

/**
 * Implements the exit builtin command functionality.
 * Returns special exit code to signal shell termination.
 * Returns 130 to signal exit request (allows cleanup).
 */
int	builtin_exit(char **args)
{
	(void)args;
	return (130);
}

/**
 * Prints all current environment variables.
 */
void	env_print(char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
}
