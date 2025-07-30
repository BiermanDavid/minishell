/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:41:13 by dabierma          #+#    #+#             */
/*   Updated: 2025/07/30 15:41:14 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

/**
 * Validates arguments for cat command.
 * Cat requires at least one file argument to avoid stdin limbo.
 */
bool	validate_cat_args(char **args)
{
	if (!args || !args[0])
		return (false);
	if (!args[1])
	{
		printf("minishell: cat: missing file argument\n");
		printf("Usage: cat <file1> [file2] ...\n");
		return (false);
	}
	return (true);
}

/**
 * Validates arguments for mkdir command.
 * Mkdir requires at least one directory name.
 */
bool	validate_mkdir_args(char **args)
{
	if (!args || !args[0])
		return (false);
	if (!args[1])
	{
		printf("minishell: mkdir: missing directory argument\n");
		printf("Usage: mkdir <directory>\n");
		return (false);
	}
	return (true);
}

/**
 * Validates arguments for ls command.
 * Ls can work without arguments (lists current directory).
 */
bool	validate_ls_args(char **args)
{
	return (args && args[0]);
}

/**
 * Validates arguments for grep command.
 * Grep requires at least a pattern argument.
 */
bool	validate_grep_args(char **args)
{
	if (!args || !args[0])
		return (false);
	if (!args[1])
	{
		printf("minishell: grep: missing pattern argument\n");
		printf("Usage: grep <pattern> [file]\n");
		return (false);
	}
	return (true);
}

/**
 * Checks if command is cat.
 * Helper function for command validation routing.
 */
bool	is_cat_command(const char *cmd)
{
	return (strcmp(cmd, "cat") == 0);
}

/**
 * Checks if command is mkdir.
 * Helper function for command validation routing.
 */
bool	is_mkdir_command(const char *cmd)
{
	return (strcmp(cmd, "mkdir") == 0);
}

/**
 * Checks if command is ls.
 * Helper function for command validation routing.
 */
bool	is_ls_command(const char *cmd)
{
	return (strcmp(cmd, "ls") == 0);
}

/**
 * Checks if command is grep.
 * Helper function for command validation routing.
 */
bool	is_grep_command(const char *cmd)
{
	return (strcmp(cmd, "grep") == 0);
}

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
void	print_command_not_found(const char *cmd)
{
	if (!cmd)
		return ;
	printf("minishell: %s: command not found\n", cmd);
}

/**
 * Prints permission denied error.
 * Used when command exists but cannot be executed.
 */
void	print_permission_denied(const char *cmd)
{
	if (!cmd)
		return ;
	printf("minishell: %s: permission denied\n", cmd);
}
