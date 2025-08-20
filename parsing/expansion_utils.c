/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 21:17:25 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/20 05:45:57 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/**
 * Extracts variable name from input string.
 * Reads alphanumeric characters and underscores.
 */
int	extract_var_name(const char *input, int input_pos, char *var_name)
{
	int	var_pos;

	var_pos = 0;
	while (input[input_pos] && (ft_isalnum(input[input_pos])
			|| input[input_pos] == '_'))
	{
		var_name[var_pos++] = input[input_pos++];
	}
	var_name[var_pos] = '\0';
	return (input_pos);
}

/**
 * Handles special variable expansion cases.
 * Processes $? for exit status and invalid variable names.
 */
int	process_special_variable(const char *input, int input_pos,
	t_exp_data *data)
{
	char	*var_value;

	if (input[input_pos] == '?')
	{
		var_value = ft_itoa(g_exit_status);
		if (var_value)
		{
			*(data->result_pos) = copy_var_value(data->result,
					*(data->result_pos), var_value);
			free(var_value);
		}
		return (input_pos + 1);
	}
	if (!input[input_pos] || (!ft_isalnum(input[input_pos])
			&& input[input_pos] != '_'))
	{
		data->result[*(data->result_pos)] = '$';
		(*(data->result_pos))++;
		return (input_pos);
	}
	return (-1);
}

/**
 * Processes escape sequences in variable expansion
 * Handles \$ and \\ escape sequences.
 */
int	process_escape_sequence(const char *input, int input_pos,
	char *result, int *result_pos)
{
	if (input[input_pos] == '\\' && input[input_pos + 1] == '$')
	{
		result[(*result_pos)++] = '$';
		return (input_pos + 2);
	}
	if (input[input_pos] == '\\' && input[input_pos + 1] == '\\')
	{
		result[(*result_pos)++] = '\\';
		return (input_pos + 2);
	}
	return (-1);
}
