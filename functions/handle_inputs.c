/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_inputs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:30:01 by dabierma          #+#    #+#             */
/*   Updated: 2025/07/30 15:40:40 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"
#include "execute.h"
#include <readline/readline.h>
#include <readline/history.h>

/**
 * Reads a line of input from the user.
 * Uses readline for input with history.
 */
char	*read_input(void)
{
	char	*input;

	input = readline("minishell> ");
	if (input && *input)
		add_history(input);
	return (input);
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

/**
 * Handles exit command processing.
 * Returns true if shell should exit.
 */
bool	handle_exit_command(char *input)
{
	if (strcmp(input, "exit") == 0)
	{
		free(input);
		printf("Goodbye!\n");
		return (true);
	}
	return (false);
}

/**
 * Processes parsed command list.
 * Handles execution without debug output.
 */
static void	process_command_list(t_cmd_list *cmd_list)
{
	if (cmd_list && cmd_list->size > 0)
	{
		execution_manager(cmd_list);
		free_cmd_list(cmd_list);
	}
}

/**
 * Processes user input through tokenization and parsing.
 * Handles the complete input processing pipeline.
 */
void	process_input(char *input)
{
	t_token		**tokens;
	int			token_count;
	t_cmd_list	*cmd_list;

	tokens = tokenize_input(input, &token_count);
	if (tokens && token_count > 0)
	{
		cmd_list = parse_command(tokens, token_count);
		process_command_list(cmd_list);
		cleanup_tokens(tokens, token_count);
	}
}
