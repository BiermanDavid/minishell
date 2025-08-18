/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_inputs_reader.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 19:21:57 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/18 18:10:23 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/**
 * Joins input with heredoc content.
 * Combines base input with heredoc data using newline separator.
 */
static char	*join_heredoc_input(char *input, char *heredoc_content)
{
	char	*temp;
	char	*complete_input;

	temp = ft_strjoin(input, "\n");
	if (temp)
	{
		complete_input = ft_strjoin(temp, heredoc_content);
		free(temp);
	}
	else
		complete_input = NULL;
	return (complete_input);
}

/**
 * Processes input and handles heredoc if present.
 * Returns complete input including heredoc content.
 */
static char	*process_heredoc_input(char *input)
{
	char	*delimiter;
	char	*heredoc_content;
	char	*complete_input;

	delimiter = extract_heredoc_delimiter(input);
	if (!delimiter)
		return (input);
	heredoc_content = read_heredoc_content(delimiter);
	if (!heredoc_content)
	{
		free(delimiter);
		return (input);
	}
	complete_input = join_heredoc_input(input, heredoc_content);
	free(heredoc_content);
	free(input);
	free(delimiter);
	return (complete_input);
}

/**
 * Reads input from interactive terminal.
 * Uses readline for input with history support.
 */
char	*read_from_tty(void)
{
	char	*input;

	input = readline("🐢minishell> ");
	if (input && *input)
		add_history(input);
	return (process_heredoc_input(input));
}

/**
 * Reads a single line from pipe input.
 * Handles non-interactive input sources.
 * buffer of 4096 is the range for bash
 */
static char	*read_pipe_line(void)
{
	char	buffer[4096];
	int		bytes_read;
	int		i;
	char	*input;

	i = 0;
	while (i < (int)(sizeof(buffer) - 1))
	{
		bytes_read = read(STDIN_FILENO, &buffer[i], 1);
		if (bytes_read <= 0)
			break ;
		if (buffer[i] == '\n')
			break ;
		i++;
	}
	if (i == 0 && bytes_read <= 0)
		return (NULL);
	buffer[i] = '\0';
	input = malloc(i + 1);
	if (!input)
		return (NULL);
	ft_strlcpy(input, buffer, i + 1);
	return (input);
}

/**
 * Reads input from pipe with heredoc processing.
 * Handles input from scripts or pipes.
 */
char	*read_from_pipe(void)
{
	char	*input;

	input = read_pipe_line();
	if (!input)
		return (NULL);
	return (process_heredoc_input(input));
}
