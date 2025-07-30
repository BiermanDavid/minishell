/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:47:44 by dabierma          #+#    #+#             */
/*   Updated: 2025/07/30 15:54:56 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/**
 * Reads one line of heredoc input.
 * Returns processed line without trailing newline.
 */
char	*read_heredoc_line(char *buffer, size_t buffer_size)
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
 * Appends a line to heredoc content buffer.
 * Handles buffer reallocation if needed.
 */
char	*append_heredoc_line(char *content, char *line, size_t *content_len,
	size_t *content_size)
{
	size_t	line_len;

	line_len = strlen(line);
	line[line_len] = '\n';
	line_len++;
	if (*content_len + line_len + 1 > *content_size)
	{
		*content_size *= 2;
		content = realloc(content, *content_size);
		if (!content)
			return (NULL);
	}
	strcpy(content + *content_len, line);
	*content_len += line_len;
	return (content);
}

/**
* Collects heredoc content from input until delimiter is found.
* Returns the collected content as a single string.
*/
char	*collect_heredoc_content(const char *delimiter)
{
	char	*content;
	char	*line;
	size_t	content_size;
	size_t	content_len;
	char	buffer[1024];

	content_size = 1024;
	content = malloc(content_size);
	if (!content)
		return (NULL);
	content[0] = '\0';
	content_len = 0;
	while (1)
	{
		line = read_heredoc_line(buffer, sizeof(buffer));
		if (!line)
			break ;
		if (strcmp(line, delimiter) == 0)
			break ;
		content = append_heredoc_line(content, line, &content_len,
				&content_size);
		if (!content)
			return (NULL);
	}
	return (content);
}
