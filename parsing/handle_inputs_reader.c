/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_inputs_reader.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 19:21:57 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/19 22:01:35 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/**
 * Reads input from interactive terminal.
 * Uses readline for input with history support.
 */
char	*read_from_tty(void)
{
	char	*input;

	input = readline("🐢minishell> ");
	if (!input)
		return (NULL);
	if (*input)
		add_history(input);
	return (input);
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
	return (input);
}
