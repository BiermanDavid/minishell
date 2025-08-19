/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 13:53:32 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/19 20:16:17 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/**
 * Checks for pipe syntax errors
 * we cant do ls > without a command after. so we check the next token
 * to make sure that its a pipe for this edge case
 */
static int	check_pipe_errors(t_token **tokens, int i, int token_count)
{
	extern int	g_exit_status;

	if (i == 0 || i == token_count - 1)
	{
		printf("minishell: parse error near `|'\n");
		g_exit_status = 2;
		return (1);
	}
	if (tokens[i + 1]->type == TOKEN_PIPE)
	{
		printf("minishell: parse error near `|'\n");
		g_exit_status = 2;
		return (1);
	}
	return (0);
}

/**
 * same as above, but the next token needs to be a word for redirects
 * also works for if we forget to do ls with 
 */
static int	check_redirect_errors(t_token **tokens, int i, int token_count)
{
	extern int	g_exit_status;

	if (i == token_count - 1 || tokens[i + 1]->type != TOKEN_WORD)
	{
		printf("minishell: parse error near `\\n'\n");
		g_exit_status = 2;
		return (1);
	}
	return (0);
}

/**
 * Checks for syntax errors. Specifically like if we had ls |
 * or ls > > filename 
 * which both require moer information to work.
 * then routes us to the correct error message.
 */
int	check_syntax_errors(t_token **tokens, int token_count)
{
	int	i;

	i = 0;
	while (i < token_count)
	{
		if (tokens[i]->type == TOKEN_PIPE)
		{
			if (check_pipe_errors(tokens, i, token_count))
				return (1);
		}
		if (tokens[i]->type >= TOKEN_REDIRECT_IN
			&& tokens[i]->type <= TOKEN_HEREDOC)
		{
			if (check_redirect_errors(tokens, i, token_count))
				return (1);
		}
		i++;
	}
	return (0);
}

int	heredoc_edge_case(t_token_data *data, int i)
{
	g_exit_status = 2;
	if (i + 1 < data->token_count
		&& data->tokens[i + 1]->type == TOKEN_WORD)
		return (i + 2);
	return (i + 1);
}
