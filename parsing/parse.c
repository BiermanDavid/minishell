/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:33:05 by dabierma          #+#    #+#             */
/*   Updated: 2025/08/20 05:40:19 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/**
 * Counts WORD tokens until hitting an operator or EOF
 */
static int	count_words_until_operator(t_token **tokens, int start,
			int token_count)
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
static void	set_command_type(t_cmd_node *cmd, t_token **tokens, int i,
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
 * Skips separator tokens in token array and gives new pos
 * separater is semicolon or ampersand
 */
static int	skip_separators(t_token **tokens, int i, int token_count)
{
	while (i < token_count && (tokens[i]->type == TOKEN_SEMICOLON
			|| tokens[i]->type == TOKEN_BACKGROUND))
		i++;
	return (i);
}

/**
 * find the first command, then process every node until you reach the next cmd
 */
static int	process_single_command(t_token_data *data, int i,
	t_cmd_list *cmd_list, char **env)
{
	t_cmd_node	*current_cmd;
	int			word_count;

	if (data->tokens[i]->type >= TOKEN_REDIRECT_IN
		&& data->tokens[i]->type <= TOKEN_HEREDOC)
		return (heredoc_edge_case(data, i));
	if (data->tokens[i]->type != TOKEN_WORD)
		return (i + 1);
	word_count = count_words_until_operator(data->tokens, i, data->token_count);
	current_cmd = create_cmd_node(CMD_SIMPLE);
	if (!current_cmd)
		return (data->token_count);
	current_cmd->cmd = extract_command_args(data->tokens, i, word_count, env);
	i += word_count;
	i = parse_redir(data->tokens, i, data->token_count, current_cmd);
	set_command_type(current_cmd, data->tokens, i, data->token_count);
	add_cmd_to_list(cmd_list, current_cmd);
	if (i < data->token_count && (data->tokens[i]->type == TOKEN_PIPE
			|| data->tokens[i]->type == TOKEN_BACKGROUND
			|| data->tokens[i]->type == TOKEN_SEMICOLON))
		i++;
	return (i);
}

/**
 * Main parser manager.
 * we check for syntax edge cases for missing commands after pipes and stuff
 * then we skip any & symbols in a loop.
 * in the loop we basicaly create the cmd list for operations like LS or cat. 
 * Then we make nodes for each argument like heloo when you do echo hello. 
 * This runs until you reach the next cmd like ls or cat, and then the process
 * repeats until EOF.
 */
t_cmd_list	*parse_command(t_token **tokens, int token_count, char **env)
{
	t_cmd_list		*cmd_list;
	int				i;
	t_token_data	data;

	if (!tokens || token_count == 0)
		return (NULL);
	if (check_syntax_errors(tokens, token_count))
		return (NULL);
	cmd_list = create_cmd_list();
	if (!cmd_list)
		return (NULL);
	data.tokens = tokens;
	data.token_count = token_count;
	i = 0;
	while (i < token_count && tokens[i]->type != TOKEN_EOF)
	{
		i = skip_separators(tokens, i, token_count);
		if (i >= token_count)
			break ;
		i = process_single_command(&data, i, cmd_list, env);
	}
	return (cmd_list);
}
