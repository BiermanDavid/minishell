/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 21:12:35 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/20 04:18:13 by dgessner         ###   ########.fr       */
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

static int	is_valid_number(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
		if (!ft_isdigit(str[i++]))
			return (0);
	return (1);
}

int	builtin_exit(char **args)
{
	extern int	g_exit_status;

	if (!args[1])
		return (g_exit_status = 0, 130);
	if (args[2])
		return (write(2, "minishell: exit: too many arguments\n", 37),
			g_exit_status = 1, 1);
	if (!is_valid_number(args[1]))
	{
		write(2, "minishell: exit: ", 17);
		write(2, args[1], ft_strlen(args[1]));
		write(2, ": numeric argument required\n", 28);
		return (g_exit_status = 255, 130);
	}
	return (g_exit_status = (unsigned char)ft_atoi(args[1]), 130);
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
