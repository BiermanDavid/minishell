/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_quotes_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 13:48:19 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/19 20:02:52 by dabierma         ###   ########.fr       */
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
 * Checks if input is a single dollar sign.
 * Returns 1 if it should be ignored, 0 otherwise.
 */
static int	handle_single_dollar(const char *temp, int temp_pos)
{
	if (temp_pos == 1 && temp[0] == '$')
		return (1);
	return (0);
}

/**
 * Processes unquoted section with variable expansion.
 * Handles mixed content expansion correctly.
 */
int	copy_unquoted_section_expanded(const char *input, int start,
		t_quote_context *ctx)
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
	if (temp_pos > 0 && !handle_single_dollar(temp, temp_pos))
	{
		expanded = expand_variables(temp, ctx->envp);
		if (expanded)
		{
			ft_strlcpy(ctx->result + *(ctx->result_pos),
				expanded, ft_strlen(expanded) + 1);
			*(ctx->result_pos) += ft_strlen(expanded);
			free(expanded);
		}
	}
	return (i);
}
