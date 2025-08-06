/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_validation.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 21:50:00 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/04 21:50:43 by dgessner         ###   ########.fr       */
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
		printf("minishell: mkdir: missing directory name\n");
		printf("Usage: mkdir <directory1> [directory2] ...\n");
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
	if (!args || !args[0])
		return (false);
	return (true);
}

/**
 * Validates arguments for grep command.
 * Grep requires a pattern and optionally files.
 */
bool	validate_grep_args(char **args)
{
	if (!args || !args[0])
		return (false);
	if (!args[1])
	{
		printf("minishell: grep: missing pattern\n");
		printf("Usage: grep <pattern> [file1] [file2] ...\n");
		return (false);
	}
	return (true);
}
