/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:50:15 by dabierma          #+#    #+#             */
/*   Updated: 2025/08/19 23:45:04 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/**
 * Determines redirection type from token type.
 * Maps token types to redirection types.
 */
static int	get_redir_type(t_token_type token_type)
{
	if (token_type == TOKEN_REDIRECT_IN)
		return (REDIR_IN);
	else if (token_type == TOKEN_REDIRECT_OUT)
		return (REDIR_OUT);
	else if (token_type == TOKEN_REDIRECT_APPEND)
		return (REDIR_APPEND);
	else if (token_type == TOKEN_HEREDOC)
		return (REDIR_HEREDOC);
	return (0);
}

/**
 * Handles heredoc redirection parsing.
 * Collects heredoc content and creates file node.
 */
static int	heredoc_redir(t_token **token, int i, int j, t_cmd_node *cmd)
{
	t_file_node	*file_node;

	i++;
	if (i >= j || token[i]->type != TOKEN_WORD)
		return (i);
	file_node = create_file_node(token[i]->value, REDIR_HEREDOC);
	if (file_node)
		add_file_to_list(cmd->files, file_node);
	return (i + 1);
}

/**
 * Handles standard redirection parsing.
 * Creates file node for input, output, or append redirections.
 */
static int	standard_redir(t_token **token, int i, int j, t_cmd_node *cmd)
{
	int			redir_type;
	t_file_node	*file_node;

	redir_type = get_redir_type(token[i]->type);
	i++;
	if (i >= j || token[i]->type != TOKEN_WORD)
		return (i);
	file_node = create_file_node(token[i]->value, redir_type);
	if (file_node)
		add_file_to_list(cmd->files, file_node);
	return (i + 1);
}

/**
 * Enhanced redirection parser that handles heredoc content.
 * Collects heredoc content when << operator is encountered.
 */
int	parse_redir(t_token **token, int start, int j, t_cmd_node *cmd)
{
	int	i;

	i = start;
	while (i < j)
	{
		if (token[i]->type == TOKEN_HEREDOC)
			i = heredoc_redir(token, i, j, cmd);
		else if (token[i]->type == TOKEN_REDIRECT_IN
			|| token[i]->type == TOKEN_REDIRECT_OUT
			|| token[i]->type == TOKEN_REDIRECT_APPEND)
			i = standard_redir(token, i, j, cmd);
		else
			break ;
	}
	return (i);
}
