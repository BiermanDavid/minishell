/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:41:13 by dabierma          #+#    #+#             */
/*   Updated: 2025/08/17 20:41:12 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

/**
 * General argument validator that routes to specific validators.
 * Returns true if arguments are valid for the command.
 */
bool	validate_command_args(char **args)
{
	if (!args || !args[0])
		return (false);
	if (is_cat_command(args[0]))
		return (validate_cat_args(args));
	else if (is_mkdir_command(args[0]))
		return (validate_mkdir_args(args));
	else if (is_ls_command(args[0]))
		return (validate_ls_args(args));
	else if (is_grep_command(args[0]))
		return (validate_grep_args(args));
	return (true);
}

/**
 * Prints command not found error.
 * Provides helpful feedback for invalid commands.
 */
int	print_command_not_found(const char *cmd)
{
	if (!cmd)
		return (127);
	printf("minishell: %s: command not found\n", cmd);
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
	printf("minishell: %s: permission denied\n", cmd);
	return (126);
}
