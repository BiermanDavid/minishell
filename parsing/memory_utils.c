/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 22:05:50 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/20 00:25:31 by dabierma         ###   ########.fr       */
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
 * Adjusted version of realloc since its not allowed
 * uses malloc to make new memory
 * then copies it over
 * then frees the old memory.
 */
void	*safe_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;
	size_t	copy_size;

	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);
	if (ptr)
	{
		if (old_size < new_size)
			copy_size = old_size;
		else
			copy_size = new_size;
		ft_memcpy(new_ptr, ptr, copy_size);
		free(ptr);
	}
	return (new_ptr);
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
