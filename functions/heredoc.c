/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:47:44 by dabierma          #+#    #+#             */
/*   Updated: 2025/07/30 17:40:44 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/**
 * Reads one line of heredoc input.
 * Returns processed line without trailing newline.
 */
char	*read_heredoc(char *buffer, size_t buffer_size)
{
	char	*line;
	size_t	line_len;

	printf("heredoc> ");
	fflush(stdout);
	if (!fgets(buffer, buffer_size, stdin))
		return (NULL);
	line = buffer;
	line_len = strlen(line);
	if (line_len > 0 && line[line_len - 1] == '\n')
	{
		line[line_len - 1] = '\0';
		line_len--;
	}
	return (line);
}

/**
 * Appends a line to heredoc data buffer.
 * Handles buffer reallocation if needed.
 */
char	*append_heredoc(char *data, char *line, size_t *len, size_t *size)
{
	size_t	line_len;

	line_len = strlen(line);
	line[line_len] = '\n';
	line_len++;
	if (*len + line_len + 1 > *size)
	{
		*size *= 2;
		data = realloc(data, *size);
		if (!data)
			return (NULL);
	}
	strcpy(data + *len, line);
	*len += line_len;
	return (data);
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

	size = 1024;
	data = malloc(size);
	if (!data)
		return (NULL);
	data[0] = '\0';
	len = 0;
	while (1)
	{
		line = read_heredoc(buffer, sizeof(buffer));
		if (!line)
			break ;
		if (strcmp(line, delimiter) == 0)
			break ;
		data = append_heredoc(data, line, &len, &size);
		if (!data)
			return (NULL);
	}
	return (data);
}
