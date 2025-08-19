/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:47:44 by dabierma          #+#    #+#             */
/*   Updated: 2025/08/19 23:45:54 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/**
 * Appends a line to heredoc data buffer.
 * Handles buffer reallocation if needed.
 */
static char	*append_heredoc(char *data, char *line, size_t *len, size_t *size)
{
	size_t	line_len;
	size_t	needed_size;

	line_len = ft_strlen(line);
	needed_size = *len + line_len + 2;
	if (needed_size > *size)
	{
		data = realloc_heredoc_buffer(data, size, needed_size, *len);
		if (!data)
			return (NULL);
	}
	ft_strlcpy(data + *len, line, line_len + 1);
	*len += line_len;
	data[*len] = '\n';
	(*len)++;
	data[*len] = '\0';
	return (data);
}

/**
 * Checks if line matches heredoc delimiter.
 * Returns 1 if line equals delimiter, 0 otherwise.
 */
static int	is_delimiter_match(const char *line, const char *delimiter)
{
	return (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
		&& ft_strlen(line) == ft_strlen(delimiter));
}

/**
 * Collects heredoc data from input until delimiter is found.
 * Returns the collected data as a single string.
 */
char	*collect_heredoc_content(const char *delimiter)
{
	char	*data;
	char	*line;
	size_t	size;
	size_t	len;
	char	buffer[1024];

	data = init_heredoc_buffer(&size, &len);
	if (!data)
		return (NULL);
	while (1)
	{
		line = read_heredoc(buffer, sizeof(buffer));
		if (!line)
			break ;
		if (is_delimiter_match(line, delimiter))
			break ;
		data = append_heredoc(data, line, &len, &size);
		if (!data)
			return (NULL);
	}
	return (data);
}
