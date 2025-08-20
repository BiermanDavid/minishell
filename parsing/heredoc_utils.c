/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 13:52:42 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/20 05:47:23 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/**
 * Reads one line of heredoc input
 * Returns processed line without trailing newline
 */
char	*read_heredoc(char *buffer, size_t buffer_size)
{
	char	c;
	size_t	i;
	int		bytes_read;

	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "heredoc> ", 9);
	i = 0;
	while (i < buffer_size - 1)
	{
		bytes_read = read(STDIN_FILENO, &c, 1);
		if (bytes_read <= 0)
			return (NULL);
		if (c == '\n')
			break ;
		buffer[i] = c;
		i++;
	}
	buffer[i] = '\0';
	return (buffer);
}

/**
 * Reallocates heredoc buffer to accommodate more data
 * Returns new buffer or NULL on failure
 */
char	*realloc_heredoc_buffer(char *data, size_t *size,
	size_t needed_size, size_t current_len)
{
	char	*new_data;
	size_t	i;

	while (*size < needed_size)
		*size *= 2;
	new_data = malloc(*size);
	if (!new_data)
		return (NULL);
	i = 0;
	while (i < current_len)
	{
		new_data[i] = data[i];
		i++;
	}
	free(data);
	return (new_data);
}

/**
 * Initializes heredoc collection buffer.
 * Sets up initial data buffer and tracking variables.
 * arbitrary buffer size based on power of 1kb. 
 * can dynamically allocate but cant find an edge case applicable
 * and whow wants to make 3 functions just to do that?
 */
char	*init_heredoc_buffer(size_t *size, size_t *len)
{
	char	*data;

	*size = 1024;
	data = malloc(*size);
	if (!data)
		return (NULL);
	data[0] = '\0';
	*len = 0;
	return (data);
}
