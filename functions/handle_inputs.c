/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_inputs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:30:01 by dabierma          #+#    #+#             */
/*   Updated: 2025/08/17 18:02:19 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"
#include "execute.h"

/**
 * Processes parsed command list.
 * Handles execution without debug output.
 */
static void	process_command_list(t_cmd_list *cmd_list, char ***envp)
{
	if (cmd_list && cmd_list->size > 0)
	{
		execution_manager(cmd_list, envp);
		free_cmd_list(cmd_list);
	}
}

/**
 * Processes user input through tokenization and parsing.
 * Handles the complete input processing pipeline.
 */
void	process_input(char *input, char ***envp)
{
	t_token		**tokens;
	int			token_count;
	t_cmd_list	*cmd_list;

	tokens = tokenize_input(input, &token_count);
	if (tokens && token_count > 0)
	{
		cmd_list = parse_command(tokens, token_count, *envp);
		process_command_list(cmd_list, envp);
		cleanup_tokens(tokens, token_count);
	}
}

/**
 * Extracts heredoc delimiter from input line.
 * Returns allocated string with delimiter or NULL if not found.
 */
static char	*extract_heredoc_delimiter(const char *input)
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
 * Reads heredoc content until delimiter is found.
 * Returns complete heredoc content as single string.
 */
static char	*read_heredoc_content(const char *delimiter)
{
	char	*content;
	char	*line_with_newline;
	char	*temp;
	char	buffer[1024];
	char	c;
	int		bytes_read;
	size_t	line_len;
	size_t	i;

	content = ft_strdup("");
	if (!content)
		return (NULL);
	while (1)
	{
		i = 0;
		while (i < sizeof(buffer) - 1)
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
			break ;
		buffer[i] = '\0';
		line_len = ft_strlen(buffer);
		if (ft_strncmp(buffer, delimiter, ft_strlen(delimiter)) == 0
			&& ft_strlen(buffer) == ft_strlen(delimiter))
			break ;
		line_with_newline = ft_strjoin(buffer, "\n");
		if (!line_with_newline)
		{
			free(content);
			return (NULL);
		}
		temp = ft_strjoin(content, line_with_newline);
		free(content);
		free(line_with_newline);
		if (!temp)
			return (NULL);
		content = temp;
	}
	return (content);
}

/**
 * Reads input and handles heredoc if present.
 * Returns complete input including heredoc content.
 */
static char	*read_complete_input(void)
{
	char	*input;
	char	*delimiter;
	char	*heredoc_content;
	char	*complete_input;
	char	*temp;
	char	buffer[1024];
	int		bytes_read;
	int		i;

	if (isatty(STDIN_FILENO))
	{
		input = readline("🐢minishell> ");
		if (input && *input)
			add_history(input);
		delimiter = extract_heredoc_delimiter(input);
		if (delimiter)
		{
			heredoc_content = read_heredoc_content(delimiter);
			if (heredoc_content)
			{
				temp = ft_strjoin(input, "\n");
				if (temp)
				{
					complete_input = ft_strjoin(temp, heredoc_content);
					free(temp);
				}
				else
					complete_input = NULL;
				free(heredoc_content);
				free(input);
				free(delimiter);
				return (complete_input);
			}
			free(delimiter);
		}
		return (input);
	}
	else
	{
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
		delimiter = extract_heredoc_delimiter(input);
		if (delimiter)
		{
			heredoc_content = read_heredoc_content(delimiter);
			if (heredoc_content)
			{
				temp = ft_strjoin(input, "\n");
				if (temp)
				{
					complete_input = ft_strjoin(temp, heredoc_content);
					free(temp);
				}
				else
					complete_input = NULL;
				free(heredoc_content);
				free(input);
				free(delimiter);
				return (complete_input);
			}
			free(delimiter);
		}
		return (input);
	}
}

/**
 * Reads a line of input from the user.
 * Uses readline for input with history.
 */
char	*read_input(void)
{
	return (read_complete_input());
}

/**
 * Checks if the input line is empty or only whitespace.
 * Returns true if line should be ignored.
 */
bool	is_empty_line(const char *input)
{
	int	i;

	if (!input)
		return (true);
	i = 0;
	while (input[i])
	{
		if (input[i] != ' ' && input[i] != '\t')
			return (false);
		i++;
	}
	return (true);
}
