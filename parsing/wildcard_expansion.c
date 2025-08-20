/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 20:45:00 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/20 05:52:02 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "parse.h"

/**
 * Initializes wildcard expansion with basic validation.
 * Returns opened directory or NULL if invalid pattern.
 */
static DIR	*init_wildcard_expansion(const char *pattern)
{
	if (!ft_strchr((char *)pattern, '*') && !ft_strchr((char *)pattern, '?'))
		return (NULL);
	return (opendir("."));
}

/**
 * Processes directory entries and collects matches.
 * Updates matches array and count as matches are found.
 */
static int	collect_matches(DIR *dir, const char *pattern, char ***matches,
		int *capacity)
{
	struct dirent	*entry;
	int				count;

	count = 0;
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] != '.' && wildcard_match(pattern, entry->d_name))
		{
			if (count >= *capacity - 1)
			{
				*matches = resize_matches(*matches, *capacity, *capacity * 2);
				if (!*matches)
					return (-1);
				*capacity *= 2;
			}
			(*matches)[count] = safe_strdup(entry->d_name);
			count++;
		}
		entry = readdir(dir);
	}
	return (count);
}

/**
 * Cleans up temporary matches array after processing.
 * Frees all match strings and the matches array itself.
 */
static void	cleanup_temp_matches(char **matches, int count)
{
	while (--count >= 0)
		free(matches[count]);
	free(matches);
}

/**
 * Allocates initial matches array for wildcard expansion.
 * Returns allocated array or NULL on failure.
 */
static char	**allocate_matches_array(DIR *dir)
{
	char	**matches;

	matches = malloc(sizeof(char *) * 64);
	if (!matches)
		closedir(dir);
	return (matches);
}

/**
 * Expands wildcard patterns to matching filenames in current directory.
 * Returns array of matches and sets match_count, or NULL if no matches.
 * arbitrary buffer size because im not making edge cases for if
 * there are more than 50 wilcards in each prompt?
 */
char	**expand_wildcards(const char *pattern, int *match_count)
{
	DIR		*dir;
	char	**matches;
	char	**result;
	int		count;
	int		capacity;

	*match_count = 0;
	dir = init_wildcard_expansion(pattern);
	if (!dir)
		return (NULL);
	matches = allocate_matches_array(dir);
	if (!matches)
		return (NULL);
	capacity = 64;
	count = collect_matches(dir, pattern, &matches, &capacity);
	closedir(dir);
	if (count <= 0)
	{
		free(matches);
		return (NULL);
	}
	*match_count = count;
	result = create_match_array(matches, count);
	cleanup_temp_matches(matches, count);
	return (result);
}
