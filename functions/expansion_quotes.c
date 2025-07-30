/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 18:35:33 by dabierma          #+#    #+#             */
/*   Updated: 2025/07/30 18:36:39 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/**
 * Copies variable value to result buffer.
 * Appends environment variable value to result string.
 */
int	copy_var_value(char *result, int result_pos, const char *var_value)
{
	int	value_len;

	if (!var_value)
		return (result_pos);
	value_len = strlen(var_value);
	strcpy(result + result_pos, var_value);
	return (result_pos + value_len);
}

/**
 * Removes surrounding quotes from string.
 * Helper function for quote processing.
 */
void	remove_quotes(char *result, const char *input, int len)
{
	if (len >= 2 && ((input[0] == '\'' && input[len - 1] == '\'')
			|| (input[0] == '"' && input[len - 1] == '"')))
	{
		strncpy(result, input + 1, len - 2);
		result[len - 2] = '\0';
	}
	else
	{
		strcpy(result, input);
	}
}

/**
 * Processes single quoted strings (no expansion).
 * Everything inside single quotes is treated literally.
 */
char	*process_single_quotes(const char *input)
{
	int		len;
	char	*result;

	if (!input)
		return (NULL);
	len = strlen(input);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	remove_quotes(result, input, len);
	return (result);
}

/**
 * Processes double quoted strings (with expansion).
 * Variables inside double quotes are expanded.
 */
char	*process_double_quotes(const char *input)
{
	int		len;
	char	*temp;
	char	*result;

	if (!input)
		return (NULL);
	len = strlen(input);
	temp = malloc(len + 1);
	if (!temp)
		return (NULL);
	remove_quotes(temp, input, len);
	result = expand_variables(temp);
	free(temp);
	return (result);
}
