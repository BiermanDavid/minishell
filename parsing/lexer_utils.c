/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 18:40:01 by dabierma          #+#    #+#             */
/*   Updated: 2025/08/20 03:24:12 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/**
 * Handles multi-character operators.
 * Returns updated position after processing operator.
 */
int	multi_char(const char *input, int pos, t_token **tokens, int i)
{
	if (input[pos] == '>' && input[pos + 1] == '>')
	{
		tokens[i] = create_token(TOKEN_REDIRECT_APPEND, ">>", pos);
		return (pos + 2);
	}
	else if (input[pos] == '<' && input[pos + 1] == '<')
	{
		tokens[i] = create_token(TOKEN_HEREDOC, "<<", pos);
		return (pos + 2);
	}
	return (pos);
}

/**
* Handles single-character operators.
* Returns updated position after processing operator.
*/
int	single_char(const char *input, int pos, t_token **tokens, int i)
{
	if (input[pos] == '|')
		tokens[i] = create_token(TOKEN_PIPE, "|", pos);
	else if (input[pos] == '&')
		tokens[i] = create_token(TOKEN_BACKGROUND, "&", pos);
	else if (input[pos] == '>')
		tokens[i] = create_token(TOKEN_REDIRECT_OUT, ">", pos);
	else if (input[pos] == '<')
		tokens[i] = create_token(TOKEN_REDIRECT_IN, "<", pos);
	else if (input[pos] == ';')
		tokens[i] = create_token(TOKEN_SEMICOLON, ";", pos);
	return (pos + 1);
}

/**
* Processes a word token.
*/
int	word_token(const char *input, int pos, t_token **tokens, int i)
{
	char	*word;
	int		word_start;

	word_start = pos;
	word = read_word(input, &pos);
	if (word)
	{
		tokens[i] = create_token(TOKEN_WORD, word, word_start);
		free(word);
	}
	return (pos);
}

/**
* Checks if operator is multi-character (plus 1 used for pos)
* Returns true for >> and << operators
*/
bool	is_multi_char_op(const char *input, int pos)
{
	return ((input[pos] == '>' && input[pos + 1] == '>')
		|| (input[pos] == '<' && input[pos + 1] == '<'));
}

/**
* Checks if character is single-character
* Returns true
*/
bool	is_single_char_op(char c)
{
	return (c == '|' || c == '&' || c == '>' || c == '<' || c == ';');
}
