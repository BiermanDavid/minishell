/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:41:35 by dabierma          #+#    #+#             */
/*   Updated: 2025/07/30 18:51:46 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/**
 * Skips whitespace characters in the input string.
 * Advances position until non-whitespace character is found.
 */
void	skip_whitespace(const char *input, int *pos)
{
	while (input[*pos] == ' ' || input[*pos] == '\t')
		(*pos)++;
}

/**
 * Creates a token with the specified type, value, and position.
 * Allocates memory and copies the value string.
 */
t_token	*create_token(t_token_type type, const char *value, int position)
{
	t_token	*token;

	token = safe_malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->position = position;
	if (value)
	{
		token->value = safe_strdup(value);
		if (!token->value)
		{
			free(token);
			return (NULL);
		}
	}
	else
		token->value = NULL;
	return (token);
}

/**
 * Processes single token from input.
 * Returns updated position and increments count.
 */
int	process_single_token(const char *input, int pos, t_token **tokens,
		int *count)
{
	if (is_multi_char_op(input, pos))
	{
		pos = multi_char(input, pos, tokens, *count);
		(*count)++;
	}
	else if (is_single_char_op(input[pos]))
	{
		pos = single_char(input, pos, tokens, *count);
		(*count)++;
	}
	else
	{
		pos = word_token(input, pos, tokens, *count);
		(*count)++;
	}
	return (pos);
}

/**
 * Simple tokenizer that handles basic shell input.
 * Recognizes words, pipes, background, and redirection operators.
 */
t_token	**tokenize_input(const char *input, int *token_count)
{
	t_token	**tokens;
	int		pos;
	int		count;

	if (!input || !token_count)
		return (NULL);
	tokens = safe_malloc(64 * sizeof(t_token *));
	if (!tokens)
		return (NULL);
	pos = 0;
	count = 0;
	while (input[pos] != '\0' && count < 63)
	{
		skip_whitespace(input, &pos);
		if (input[pos] == '\0')
			break ;
		pos = process_single_token(input, pos, tokens, &count);
	}
	tokens[count] = create_token(TOKEN_EOF, NULL, pos);
	count++;
	*token_count = count;
	return (tokens);
}

/**
 * Frees memory for a single token.
 * Cleans up the value string and the token structure.
 */
void	destroy_token(t_token *token)
{
	if (!token)
		return ;
	if (token->value)
		free(token->value);
	free(token);
}
