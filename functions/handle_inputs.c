/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_inputs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:30:01 by dabierma          #+#    #+#             */
/*   Updated: 2025/08/03 03:33:11 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"
#include "execute.h"
#include <readline/readline.h>
#include <readline/history.h>

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
#ifdef DEBUG_MOD
		print_parsed_commands(cmd_list);
#endif
		process_command_list(cmd_list, envp);
		cleanup_tokens(tokens, token_count);
	}
}


/**
 * Reads a line of input from the user.
 * Uses readline for input with history.
 */
char	*read_input(void)
{
	char	*input;

	input = readline("🐢minishell> ");
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
