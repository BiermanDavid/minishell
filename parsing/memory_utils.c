/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 22:05:50 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/18 17:52:26 by dabierma         ###   ########.fr       */
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

void	*safe_realloc(void *ptr, size_t size)
{
	void	*new_ptr;

	new_ptr = realloc(ptr, size);
	if (!new_ptr && size > 0)
	{
		free(ptr);
		write(2, "Error: Memory allocation failed\n", 33);
		exit(1);
	}
	return (new_ptr);
}
