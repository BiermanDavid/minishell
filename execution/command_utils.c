/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 21:55:29 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/04 22:40:08 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

/**
 * Checks if command is cat.
 */
bool	is_cat_command(const char *cmd)
{
	if (!cmd)
		return (false);
	return (ft_strncmp(cmd, "cat", 4) == 0 && ft_strlen(cmd) == 3);
}

/**
 * Checks if command is mkdir.
 */
bool	is_mkdir_command(const char *cmd)
{
	if (!cmd)
		return (false);
	return (ft_strncmp(cmd, "mkdir", 6) == 0 && ft_strlen(cmd) == 5);
}

/**
 * Checks if command is ls.
 */
bool	is_ls_command(const char *cmd)
{
	if (!cmd)
		return (false);
	return (ft_strncmp(cmd, "ls", 3) == 0 && ft_strlen(cmd) == 2);
}

/**
 * Checks if command is grep.
 */
bool	is_grep_command(const char *cmd)
{
	if (!cmd)
		return (false);
	return (ft_strncmp(cmd, "grep", 5) == 0 && ft_strlen(cmd) == 4);
}
