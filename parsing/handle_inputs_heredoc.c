/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_inputs_heredoc.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 21:29:11 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/18 14:02:30 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/**
 * Extracts heredoc delimiter from input line.
 * Returns allocated string with delimiter or NULL if not found.
 */
char	*extract_heredoc_delimiter(const char *input)
{
	char	*heredoc_pos;
	char	*delimiter_start;
	char	*delimiter_end;
	int		len;
	char	*delimiter;

	heredoc_pos = ft_strnstr(input, "<<", ft_strlen(input));
	if (!heredoc_pos)
		return (NULL);
	delimiter_start = heredoc_pos + 2;
	while (*delimiter_start == ' ' || *delimiter_start == '\t')
		delimiter_start++;
	if (!*delimiter_start)
		return (NULL);
	delimiter_end = delimiter_start;
	while (*delimiter_end && *delimiter_end != ' ' && *delimiter_end != '\t'
		&& *delimiter_end != '\n')
		delimiter_end++;
	len = delimiter_end - delimiter_start;
	delimiter = malloc(len + 1);
	if (!delimiter)
		return (NULL);
	ft_strlcpy(delimiter, delimiter_start, len + 1);
	return (delimiter);
}

/**
 * Reads a single line from stdin into buffer.
 * Returns buffer on success, NULL on EOF or error.
 */
static char	*read_line_from_stdin(char *buffer, int buffer_size)
{
	int		bytes_read;
	char	c;
	size_t	i;

	i = 0;
	while (i < (size_t)(buffer_size - 1))
	{
		bytes_read = read(STDIN_FILENO, &c, 1);
		if (bytes_read <= 0)
			break ;
		if (c == '\n')
			break ;
		buffer[i] = c;
		i++;
	}
	if (bytes_read <= 0 && i == 0)
		return (NULL);
	buffer[i] = '\0';
	return (buffer);
}

/**
 * Checks if buffer matches the heredoc delimiter exactly.
 * Returns 1 if match, 0 otherwise.
 */
static int	check_delimiter_match(const char *buffer, const char *delimiter)
{
	if (ft_strncmp(buffer, delimiter, ft_strlen(delimiter)) == 0
		&& ft_strlen(buffer) == ft_strlen(delimiter))
		return (1);
	return (0);
}

/**
 * Appends a line to heredoc content with newline.
 * Returns new content string or NULL on failure.
 */
static char	*append_heredoc_line(char *content, const char *buffer)
{
	char	*line_with_newline;
	char	*temp;

	line_with_newline = ft_strjoin(buffer, "\n");
	if (!line_with_newline)
		return (NULL);
	temp = ft_strjoin(content, line_with_newline);
	free(line_with_newline);
	return (temp);
}

/**
 * Reads heredoc content until delimiter is found.
 * Returns complete heredoc content as single string.
 */
char	*read_heredoc_content(const char *delimiter)
{
	char	*content;
	char	*temp;
	char	buffer[1024];

	content = ft_strdup("");
	if (!content)
		return (NULL);
	while (1)
	{
		if (!read_line_from_stdin(buffer, sizeof(buffer)))
			break ;
		if (check_delimiter_match(buffer, delimiter))
			break ;
		temp = append_heredoc_line(content, buffer);
		free(content);
		if (!temp)
			return (NULL);
		content = temp;
	}
	return (content);
}
