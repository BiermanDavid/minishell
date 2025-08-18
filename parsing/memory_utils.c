/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 22:05:50 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/04 22:42:29 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/**
 * Safe malloc wrapper with error handling.
 * Exits program if allocation fails.
 */
void	*safe_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
	{
		printf("Error: Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}
	return (ptr);
}

/**
 * Safe strdup wrapper with error handling.
 * Exits program if allocation fails.
 */
char	*safe_strdup(const char *s)
{
	char	*dup;

	if (!s)
		return (NULL);
	dup = ft_strdup(s);
	if (!dup)
	{
		printf("Error: Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}
	return (dup);
}
