/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_inputs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:30:01 by dabierma          #+#    #+#             */
/*   Updated: 2025/08/18 17:51:41 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"
#include "execute.h"

/**
 * Final step after parsing, push full parsed content to Execution manager
 * Then free the left over memory.
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
 * split the readline arguments into tokens,
 * parse for specific cmds(expansion heredoc etc...)
 * 
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
 * //TODO merge wrapper into read input?
 */
char	*read_complete_input(void)
{
	if (isatty(STDIN_FILENO))
		return (read_from_tty());
	else
		return (read_from_pipe());
}

char	*read_input(void)
{
	return (read_complete_input());
}

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
