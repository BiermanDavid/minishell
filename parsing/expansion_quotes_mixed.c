/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_quotes_mixed.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 13:48:48 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/20 00:50:02 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

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
 * Processes quoted section in mixed content.
 * Handles single or double quoted sections appropriately.
 */
int	process_quoted_part(const char *input, int pos, t_quote_context *ctx)
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
		processed = process_double_quotes(temp, ctx->envp);
	if (processed)
	{
		ft_strlcpy(ctx->result + *(ctx->result_pos),
			processed, ft_strlen(processed) + 1);
		*(ctx->result_pos) += ft_strlen(processed);
		free(processed);
	}
	return (pos);
}

/**
 * Processes mixed quoted and unquoted content.
 * Handles tokens like hello'world' correctly.
 */
char	*process_mixed_content(const char *input, char **envp)
{
	char			*result;
	int				input_pos;
	int				result_pos;
	t_quote_context	ctx;

	(void)envp;
	result = ft_calloc(1024, sizeof(char));
	if (!result)
		return (NULL);
	input_pos = 0;
	result_pos = 0;
	ctx.result = result;
	ctx.result_pos = &result_pos;
	ctx.envp = envp;
	while (input[input_pos])
	{
		if (input[input_pos] == '\'' || input[input_pos] == '"')
			input_pos = process_quoted_part(input, input_pos, &ctx);
		else
			input_pos = copy_unquoted_section_expanded(input, input_pos, &ctx);
	}
	result[result_pos] = '\0';
	return (result);
}
