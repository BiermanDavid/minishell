/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:41:13 by dabierma          #+#    #+#             */
/*   Updated: 2025/08/19 21:32:46 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

/**
 * Prints command not found error.
 * Provides helpful feedback for invalid commands.
 */
int	print_command_not_found(const char *cmd)
{
	if (!cmd)
		return (127);
	write(STDERR_FILENO, "minishell: ", 11);
	write(STDERR_FILENO, cmd, ft_strlen(cmd));
	write(STDERR_FILENO, ": command not found\n", 20);
	return (127);
}

/**
 * Prints permission denied error.
 * Used when command exists but cannot be executed.
 */
int	print_permission_denied(const char *cmd)
{
	if (!cmd)
		return (126);
	write(STDERR_FILENO, "minishell: ", 11);
	write(STDERR_FILENO, cmd, ft_strlen(cmd));
	write(STDERR_FILENO, ": permission denied\n", 20);
	return (126);
}

/**
 * Prints directory does not exist error.
 * Used when cd target directory cannot be found.
 */
int	print_directory_not_found(const char *dir)
{
	if (!dir)
		return (1);
	write(STDERR_FILENO, "cd: ", 4);
	write(STDERR_FILENO, dir, ft_strlen(dir));
	write(STDERR_FILENO, ": No such file or directory\n", 28);
	return (1);
}
