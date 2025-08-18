/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:41:35 by dabierma          #+#    #+#             */
/*   Updated: 2025/08/18 17:52:06 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

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

static t_token	**ensure_capacity(t_token **tokens, int count, int *buffer)
{
	t_token	**new_tokens;

	if (count < *buffer - 1)
		return (tokens);
	*buffer *= 2;
	new_tokens = safe_realloc(tokens, *buffer * sizeof(t_token *));
	return (new_tokens);
}

static t_token	**init_tokens(int *buffer)
{
	*buffer = 16;
	return (safe_malloc(*buffer * sizeof(t_token *)));
}

/**
 * Simple tokenizer that handles basic shell input.
 * Recognizes words, pipes, background, and redirection 
 * increases in size buffer based on length of readline arg
 */

t_token	**tokenize_input(const char *input, int *token_count)
{
	t_token	**tokens;
	int		pos;
	int		count;
	int		buffer;

	if (!input || !token_count)
		return (NULL);
	tokens = init_tokens(&buffer);
	if (!tokens)
		return (NULL);
	pos = 0;
	count = 0;
	while (input[pos] != '\0')
	{
		skip_whitespace(input, &pos);
		if (input[pos] == '\0')
			break ;
		tokens = ensure_capacity(tokens, count, &buffer);
		if (!tokens)
			return (NULL);
		pos = process_single_token(input, pos, tokens, &count);
	}
	tokens[count] = create_token(TOKEN_EOF, NULL, pos);
	*token_count = count + 1;
	return (tokens);
}
