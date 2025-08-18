/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 20:52:00 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/17 21:01:00 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "parse.h"

/**
 * Handles asterisk wildcard matching with recursive search.
 * Returns 1 if pattern matches from current position, 0 otherwise.
 */
static int	match_asterisk(const char *pattern, const char *str)
{
	pattern++;
	if (!*pattern)
		return (1);
	while (*str)
	{
		if (wildcard_match(pattern, str))
			return (1);
		str++;
	}
	return (0);
}

/**
 * Matches wildcard patterns with strings using * and ? wildcards.
 * Returns 1 if pattern matches, 0 otherwise.
 */
int	wildcard_match(const char *pattern, const char *str)
{
	while (*pattern && *str)
	{
		if (*pattern == '*')
			return (match_asterisk(pattern, str));
		else if (*pattern == '?' || *pattern == *str)
		{
			pattern++;
			str++;
		}
		else
			return (0);
	}
	while (*pattern == '*')
		pattern++;
	return (!*pattern && !*str);
}

/**
 * Cleans up partially allocated match array on failure.
 * Frees all previously allocated strings and the array itself.
 */
static void	cleanup_match_array(char **result, int count)
{
	while (--count >= 0)
		free(result[count]);
	free(result);
}

/**
 * Creates an array of string matches from wildcard results.
 * Returns allocated array or NULL on failure.
 */
char	**create_match_array(char **matches, int count)
{
	char	**result;
	int		i;

	result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (i < count)
	{
		result[i] = safe_strdup(matches[i]);
		if (!result[i])
		{
			cleanup_match_array(result, i);
			return (NULL);
		}
		i++;
	}
	result[count] = NULL;
	return (result);
}

/**
 * Resizes the matches array to accommodate more entries.
 * Returns new array or NULL on failure.
 */
char	**resize_matches(char **old_matches, int old_capacity, int new_capacity)
{
	char	**new_matches;
	int		i;

	new_matches = malloc(sizeof(char *) * new_capacity);
	if (!new_matches)
		return (NULL);
	i = 0;
	while (i < old_capacity && old_matches[i])
	{
		new_matches[i] = old_matches[i];
		i++;
	}
	free(old_matches);
	return (new_matches);
}
