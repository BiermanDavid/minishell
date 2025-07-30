/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:41:24 by dabierma          #+#    #+#             */
/*   Updated: 2025/07/30 15:41:25 by dabierma         ###   ########.fr       */
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
	while (input[input_pos] && (isalnum(input[input_pos])
			|| input[input_pos] == '_'))
	{
		var_name[var_pos++] = input[input_pos++];
	}
	var_name[var_pos] = '\0';
	return (input_pos);
}

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
 * Processes a variable expansion in the input.
 * Handles $VAR expansion and updates positions.
 */
int	process_variable(const char *input, int input_pos, char *result,
		int result_pos)
{
	char	var_name[256];
	char	*var_value;

	input_pos++;
	input_pos = extract_var_name(input, input_pos, var_name);
	var_value = getenv(var_name);
	result_pos = copy_var_value(result, result_pos, var_value);
	return (result_pos);
}

/**
 * Expands environment variables in a string.
 * Replaces $VAR with the value of environment variable VAR.
 */
char	*expand_variables(const char *input)
{
	char	*result;
	int		result_pos;
	int		input_pos;

	if (!input)
		return (NULL);
	result = malloc(1024);
	if (!result)
		return (NULL);
	result_pos = 0;
	input_pos = 0;
	while (input[input_pos])
	{
		if (input[input_pos] == '$')
			result_pos = process_variable(input, input_pos, result,
					result_pos);
		else
			result[result_pos++] = input[input_pos];
		input_pos++;
	}
	result[result_pos] = '\0';
	return (result);
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

/**
 * Determines quote type and processes accordingly.
 * Routes to appropriate quote handler based on quote type.
 */
char	*process_quoted_string(const char *input)
{
	if (!input || strlen(input) == 0)
		return (NULL);
	if (input[0] == '\'')
		return (process_single_quotes(input));
	else if (input[0] == '"')
		return (process_double_quotes(input));
	else
		return (expand_variables(input));
}

/**
 * Expands a single token based on its content.
 * Handles quotes and variable expansion appropriately.
 */
char	*expand_token(const char *token_value)
{
	if (!token_value)
		return (NULL);
	return (process_quoted_string(token_value));
}
