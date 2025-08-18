/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 21:12:35 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/04 21:50:24 by dgessner         ###   ########.fr       */
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
 * Terminates the shell process with exit code 0.
 * Returns 0 (though this is never reached due to exit).
 */
int	builtin_exit(char **args)
{
	(void)args;
	exit(0);
	return (0);
}
