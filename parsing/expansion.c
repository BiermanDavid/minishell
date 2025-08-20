/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:41:24 by dabierma          #+#    #+#             */
/*   Updated: 2025/08/20 05:29:46 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/**
 * Processes a variable expansion in the input
 * Handles $VAR expansion
 */
static int	process_variable(const char *input, int input_pos,
	t_exp_data *data, char **envp)
{
	char	var_name[256];
	char	*var_value;
	int		special_result;

	input_pos++;
	special_result = process_special_variable(input, input_pos, data);
	if (special_result != -1)
		return (special_result);
	input_pos = extract_var_name(input, input_pos, var_name);
	var_value = env_get(envp, var_name);
	if (var_value)
		*(data->result_pos) = copy_var_value(data->result,
				*(data->result_pos), var_value);
	return (input_pos);
}

/**
 * Initializes expansion data structure for variable processing.
 * Sets up result buffer and position tracking.
 */
static char	*init_expansion(const char *input, t_exp_data *data,
	int *result_pos, int *input_pos)
{
	char	*result;

	if (!input)
		return (NULL);
	result = ft_calloc(1024, sizeof(char));
	if (!result)
		return (NULL);
	*result_pos = 0;
	*input_pos = 0;
	data->result = result;
	data->result_pos = result_pos;
	return (result);
}

/**
 * Expands environment variables in a string.
 * Replaces $VAR with the value of environment variable VAR.
 */
char	*expand_variables(const char *input, char **envp)
{
	char		*result;
	int			result_pos;
	int			input_pos;
	t_exp_data	data;
	int			escape_result;

	result = init_expansion(input, &data, &result_pos, &input_pos);
	if (!result)
		return (NULL);
	while (input[input_pos])
	{
		escape_result = process_escape_sequence(input, input_pos,
				result, &result_pos);
		if (escape_result != -1)
			input_pos = escape_result;
		else if (input[input_pos] == '$')
			input_pos = process_variable(input, input_pos, &data, envp);
		else
			result[result_pos++] = input[input_pos++];
	}
	result[result_pos] = '\0';
	return (result);
}

/**
 * Determines quote type and processes accordingly.
 * Routes to appropriate quote handler based on content type.
 */
static char	*process_quoted_string(const char *input, char **envp)
{
	if (!input || ft_strlen(input) == 0)
		return (NULL);
	if (has_mixed_quotes(input))
		return (process_mixed_content(input, envp));
	if (input[0] == '\'')
		return (process_single_quotes(input));
	else if (input[0] == '"')
		return (process_double_quotes(input, envp));
	else
		return (expand_variables(input, envp));
}

/**
 * Expands a single token based on its content.
 * Handles quotes and variable expansion appropriately.
 */
char	*expand_token(const char *token_value, char **envp)
{
	if (!token_value)
		return (NULL);
	return (process_quoted_string(token_value, envp));
}
