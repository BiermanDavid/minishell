/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 18:35:33 by dabierma          #+#    #+#             */
/*   Updated: 2025/08/03 03:32:13 by dgessner         ###   ########.fr       */
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
	value_len = ft_strlen(var_value);
	ft_strlcpy(result + result_pos, var_value, value_len + 1);
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
		ft_strlcpy(result, input + 1, len - 1);
		result[len - 2] = '\0';
	}
	else
	{
		ft_strlcpy(result, input, len + 1);
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
	len = ft_strlen(input);
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
char	*process_double_quotes(const char *input, char **envp)
{
	int		len;
	char	*temp;
	char	*result;

	if (!input)
		return (NULL);
	len = ft_strlen(input);
	temp = malloc(len + 1);
	if (!temp)
		return (NULL);
	remove_quotes(temp, input, len);
	result = expand_variables(temp, envp);
	free(temp);
	return (result);
}
