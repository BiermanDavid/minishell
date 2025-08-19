/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:41:35 by dabierma          #+#    #+#             */
/*   Updated: 2025/08/20 00:30:04 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/**
 * Creates a token with the specified type and its location
 * Allocates memory and copies the value
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
 * one token at a time, check if its multi character or single, then increment
 * uses appropraite funciton call for << >> or other things like
 * ; < > & pipes. These all need to then be categorized so we
 * know which element of the meta struct it belongs to
 * basically just finds the correct token type 
 */
static int	process_single_token(const char *input, int pos, t_token **tokens,
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
 * Sets up a dynamic buffer size to allow tokens to be made for
 * very very long prompts.
 */
static t_token	**realloc_wrapper(t_token **tokens, int count, int *buffer)
{
	t_token	**new_tokens;
	int		old_buffer;

	if (count < *buffer - 1)
		return (tokens);
	old_buffer = *buffer;
	*buffer *= 2;
	new_tokens = safe_realloc(tokens, old_buffer * sizeof(t_token *),
			(*buffer) * sizeof(t_token *));
	return (new_tokens);
}

/**
 * Wrapper function for memory allocation of tokens.
 */
static t_token	**init_tokens(int *buffer)
{
	*buffer = 16;
	return (safe_malloc(*buffer * sizeof(t_token *)));
}

/**
 * Simple tokenizer that handles basic shell input.
 * Recognizes words, pipes, background, and redirection 
 * increases in size buffer based on length of readline arg
 * used realloc to make space for very long lines
 * originally hard coded to take a lenght of 100 but realized
 * that tokens stopped being made for very long prompts.
 * Dont hardcode token buffer size lol
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
		tokens = realloc_wrapper(tokens, count, &buffer);
		if (!tokens)
			return (NULL);
		pos = process_single_token(input, pos, tokens, &count);
	}
	tokens[count] = create_token(TOKEN_EOF, NULL, pos);
	*token_count = count + 1;
	return (tokens);
}
