/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_inputs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:30:01 by dabierma          #+#    #+#             */
/*   Updated: 2025/08/20 05:41:10 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"
#include "execute.h"

/**
 * Cleanup function for token array.
 * Frees all tokens and the array itself.
 */
static void	cleanup_tokens(t_token **tokens, int token_count)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (i < token_count)
	{
		destroy_token(tokens[i]);
		i++;
	}
	free(tokens);
}

/**
 * Final step after parsing, push full parsed content to Execution manager
 * Then free the left over memory. 130 is signal code for executing
 * the command exit. we only have this so we can avoid a valgrind issue
 */
static int	process_command_list(t_cmd_list *cmd_list, char ***envp)
{
	int	result;

	result = 0;
	if (cmd_list)
	{
		if (cmd_list->size > 0)
		{
			result = execution_manager(cmd_list, envp);
			if (result == -130)
			{
				free_cmd_list(cmd_list);
				return (-130);
			}
		}
		free_cmd_list(cmd_list);
	}
	return (result);
}

/**
 * split the readline arguments into tokens,
 * parse for specific cmds(expansion heredoc etc...)
 *  makes something like ls-la | grep beans output.txt into
 * ls token -la token PIPE grep.
 * then we make nodes in the comand list to get beans as an executable for grep
 */
int	process_input(char *input, char ***envp)
{
	t_token		**tokens;
	int			token_count;
	t_cmd_list	*cmd_list;
	int			result;

	tokens = tokenize_input(input, &token_count);
	if (tokens && token_count > 0)
	{
		cmd_list = parse_command(tokens, token_count, *envp);
		result = process_command_list(cmd_list, envp);
		cleanup_tokens(tokens, token_count);
		return (result);
	}
	return (0);
}

/**
 * tty checks for terminal input and uses STDIN_FILNO to return 1 like bash
 * pipe does same thing but returns 0 in STDIN (basically read from non terminal)
 * so pipe allows usage of stuff like runing a .sh or doing pwd
 * GNU unistd library made in 1980s wow
 * tty=teleTYpewriter
 * this is part of shell loop and is always allow us to type with readline
 */
char	*read_input(void)
{
	if (isatty(STDIN_FILENO))
		return (read_from_tty());
	else
		return (read_from_pipe());
}

/**
 * Edge case bool for if the line is empty return true
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
