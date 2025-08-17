/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 18:35:33 by dabierma          #+#    #+#             */
/*   Updated: 2025/08/17 21:16:41 by dgessner         ###   ########.fr       */
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

/**
 * Checks if token contains mixed quoted and unquoted content.
 * Returns true if token needs special mixed processing.
 */
int	has_mixed_quotes(const char *input)
{
	int	i;
	int	quote_found;

	i = 0;
	quote_found = 0;
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '"')
		{
			if (i > 0 || quote_found)
				return (1);
			quote_found = 1;
			while (input[i] && input[i + 1] && input[i + 1] != input[i])
				i++;
			if (input[i + 1])
				i += 2;
			if (input[i])
				return (1);
		}
		else
			i++;
	}
	return (0);
}

/**
 * Processes mixed quoted and unquoted content.
 * Handles tokens like hello'world' correctly.
 */
char	*process_mixed_content(const char *input, char **envp)
{
	char	*result;
	int		input_pos;
	int		result_pos;

	result = ft_calloc(1024, sizeof(char));
	if (!result)
		return (NULL);
	input_pos = 0;
	result_pos = 0;
	while (input[input_pos])
	{
		if (input[input_pos] == '\'' || input[input_pos] == '"')
			input_pos = process_quoted_part(input, input_pos, result,
					&result_pos, envp);
		else
			input_pos = copy_unquoted_section_expanded(input, input_pos, result,
					&result_pos, envp);
	}
	result[result_pos] = '\0';
	return (result);
}

/**
 * Processes quoted section in mixed content.
 * Handles single or double quoted sections appropriately.
 */
int	process_quoted_part(const char *input, int pos, char *result,
		int *result_pos, char **envp)
{
	char	quote_char;
	char	temp[1024];
	char	*processed;
	int		temp_pos;

	quote_char = input[pos];
	temp_pos = 0;
	temp[temp_pos++] = input[pos++];
	while (input[pos] && input[pos] != quote_char)
		temp[temp_pos++] = input[pos++];
	if (input[pos] == quote_char)
		temp[temp_pos++] = input[pos++];
	temp[temp_pos] = '\0';
	if (quote_char == '\'')
		processed = process_single_quotes(temp);
	else
		processed = process_double_quotes(temp, envp);
	if (processed)
	{
		ft_strlcpy(result + *result_pos, processed, ft_strlen(processed) + 1);
		*result_pos += ft_strlen(processed);
		free(processed);
	}
	return (pos);
}

/**
 * Processes unquoted section in mixed content.
 * Copies unquoted characters to result.
 */
int	copy_unquoted_section(const char *input, int start, char *result,
		int *result_pos)
{
	int	i;

	i = start;
	while (input[i] && input[i] != '\'' && input[i] != '"')
	{
		result[(*result_pos)++] = input[i];
		i++;
	}
	return (i);
}

/**
 * Processes unquoted section with variable expansion.
 * Handles mixed content expansion correctly.
 */
int	copy_unquoted_section_expanded(const char *input, int start, char *result,
		int *result_pos, char **envp)
{
	int		i;
	char	temp[1024];
	char	*expanded;
	int		temp_pos;

	i = start;
	temp_pos = 0;
	while (input[i] && input[i] != '\'' && input[i] != '"')
	{
		temp[temp_pos++] = input[i];
		i++;
	}
	temp[temp_pos] = '\0';
	if (temp_pos > 0)
	{
		if (temp_pos == 1 && temp[0] == '$')
		{
			return (i);
		}
		expanded = expand_variables(temp, envp);
		if (expanded)
		{
			ft_strlcpy(result + *result_pos, expanded, ft_strlen(expanded) + 1);
			*result_pos += ft_strlen(expanded);
			free(expanded);
		}
	}
	return (i);
}
