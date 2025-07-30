/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 23:33:07 by dgessner          #+#    #+#             */
/*   Updated: 2025/03/17 14:41:37 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static unsigned int	count_tokens(char const *s, char c)
{
	unsigned int	count;

	count = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			count++;
			while (*s && *s != c)
				s++;
		}
	}
	return (count);
}

static char	*get_word(char const **s, char c)
{
	unsigned int	len;
	char			*word;

	while (**s == c)
		(*s)++;
	len = 0;
	while ((*s)[len] && (*s)[len] != c)
		len++;
	word = malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	ft_strlcpy(word, *s, len + 1);
	*s += len;
	return (word);
}

static void	free_memory(char **result, unsigned int i)
{
	while (i > 0)
	{
		i--;
		free(result[i]);
	}
	free(result);
}

char	**ft_split(char const *s, char c)
{
	char			**result;
	unsigned int	tokens;
	unsigned int	i;

	if (!s)
		return (NULL);
	tokens = count_tokens(s, c);
	result = malloc(sizeof(char *) * (tokens + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (i < tokens)
	{
		result[i] = get_word(&s, c);
		if (!result[i])
		{
			free_memory(result, i);
			return (NULL);
		}
		i++;
	}
	result[i] = NULL;
	return (result);
}
