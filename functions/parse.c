/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:33:05 by dabierma          #+#    #+#             */
/*   Updated: 2025/08/03 02:58:34 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/**
 * Counts WORD tokens until hitting an operator or EOF.
 * Used to determine command argument count.
 */
int	count_words_until_operator(t_token **tokens, int start, int token_count)
{
	int	count;
	int	i;

	count = 0;
	i = start;
	while (i < token_count && tokens[i]->type == TOKEN_WORD)
	{
		count++;
		i++;
	}
	return (count);
}

/**
 * Determines command type based on following token.
 * Sets appropriate command type for execution.
 */
void	set_command_type(t_cmd_node *cmd, t_token **tokens, int i,
		int token_count)
{
	if (i < token_count)
	{
		if (tokens[i]->type == TOKEN_PIPE)
			cmd->cmd_type = CMD_PIPE;
		else if (tokens[i]->type == TOKEN_BACKGROUND)
			cmd->cmd_type = CMD_BACKGROUND;
	}
}

/**
 * Skips separator tokens in token array.
 * Returns updated index after skipping separators.
 */
int	skip_separators(t_token **tokens, int i, int token_count)
{
	while (i < token_count && (tokens[i]->type == TOKEN_SEMICOLON
			|| tokens[i]->type == TOKEN_BACKGROUND))
		i++;
	return (i);
}

/**
 * Processes a single command from tokens.
 * Creates command node and parses arguments and redirections.
 */
int	process_single_command(t_token **tokens, int i, int token_count,
	t_cmd_list *cmd_list, char **env)
{
	t_cmd_node	*current_cmd;
	int			word_count;

	if (tokens[i]->type != TOKEN_WORD)
		return (i + 1);
	word_count = count_words_until_operator(tokens, i, token_count);
	current_cmd = create_cmd_node(CMD_SIMPLE);
	if (!current_cmd)
		return (token_count);
	current_cmd->cmd = extract_command_args(tokens, i, word_count, env);
	i += word_count;
	i = parse_redir(tokens, i, token_count, current_cmd);
	set_command_type(current_cmd, tokens, i, token_count);
	add_cmd_to_list(cmd_list, current_cmd);
	if (i < token_count && (tokens[i]->type == TOKEN_PIPE
			|| tokens[i]->type == TOKEN_BACKGROUND
			|| tokens[i]->type == TOKEN_SEMICOLON))
		i++;
	return (i);
}

/**
 * Main parser - creates clear pipe chains for execution.
 * CMD_PIPE means "pipes to next command", CMD_PIPE_END means "last in chain".
 */
t_cmd_list	*parse_command(t_token **tokens, int token_count, char **env)
{
	t_cmd_list	*cmd_list;
	int			i;

	if (!tokens || token_count == 0)
		return (NULL);
	cmd_list = create_cmd_list();
	if (!cmd_list)
		return (NULL);
	i = 0;
	while (i < token_count && tokens[i]->type != TOKEN_EOF)
	{
		i = skip_separators(tokens, i, token_count);
		if (i >= token_count)
			break ;
		i = process_single_command(tokens, i, token_count, cmd_list, env);
	}
	return (cmd_list);
}
