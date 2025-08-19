/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 18:39:19 by dabierma          #+#    #+#             */
/*   Updated: 2025/08/20 00:39:57 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"
/**
 * Checks if character should break word reading
 * Returns true for delimiters and operators
 */
static bool	is_word_delimiter(char c)
{
	return (c == ' ' || c == '\t' || c == '|' || c == '&'
		|| c == '>' || c == '<' || c == ';');
}

/**
 * Handles quotes
 * Updates position and quote status as true
 * in_q = in quotes && q_char = quote character
 */
static void	handle_quote(const char *input, int *pos, bool *in_q, char *q_char)
{
	if (!*in_q)
	{
		if (input[*pos] == '"' || input[*pos] == '\'')
		{
			*q_char = input[*pos];
			*in_q = true;
		}
	}
	else
	{
		if (input[*pos] == *q_char)
		{
			*in_q = false;
			*q_char = '\0';
		}
	}
}

/**
 * Calculates word length with quote handling.
 * Returns the length of the word token.
 * This function handles edge cases for quotes insie of words
 * I.E. Hello'world' has a quote in the world
 * so if we echo hello'world' it should consdier a compound word to be echoed
 * instead of echoing only world.
 */
static int	calculate_word_length(const char *input, int *pos)
{
	int		start;
	bool	in_q;
	char	q_char;

	start = *pos;
	in_q = false;
	q_char = '\0';
	while (input[*pos])
	{
		handle_quote(input, pos, &in_q, &q_char);
		if (!in_q && is_word_delimiter(input[*pos]))
			break ;
		(*pos)++;
	}
	return (*pos - start);
}

/**
 * Copies word from input to memory buffer
 */
static void	copy_word(char *word, const char *input, int start, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		word[i] = input[start + i];
		i++;
	}
	word[len] = '\0';
}

/**
 * Reads a word token from input
 * Handles single quotes, double quotes with calc word len func cal
 */
char	*read_word(const char *input, int *pos)
{
	int		start;
	int		len;
	char	*word;

	start = *pos;
	len = calculate_word_length(input, pos);
	word = malloc(len + 1);
	if (!word)
		return (NULL);
	copy_word(word, input, start, len);
	return (word);
}
