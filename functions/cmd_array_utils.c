/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_array_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 21:15:00 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/17 21:15:34 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "parse.h"

/**
 * Counts the number of arguments in a command array.
 * Returns the count of non-NULL string pointers.
 */
int	count_args(char **args)
{
	int	count;

	count = 0;
	while (args && args[count])
		count++;
	return (count);
}

/**
 * Copies replacement arguments to new command array.
 * Helper function for rebuild_cmd_array.
 */
void	copy_replacements(char **new_cmd, int *new_i,
		char **replacements, int replacement_count)
{
	int	j;

	j = 0;
	while (j < replacement_count)
	{
		new_cmd[*new_i] = safe_strdup(replacements[j]);
		(*new_i)++;
		j++;
	}
}

/**
 * Copies one old command argument to new command array.
 * Helper function for rebuild_cmd_array.
 */
void	copy_single_arg(char **new_cmd, int *new_i, char *old_arg)
{
	new_cmd[*new_i] = safe_strdup(old_arg);
	(*new_i)++;
}
