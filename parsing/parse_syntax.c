/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 13:53:32 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/18 13:53:38 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/**
 * Checks for pipe syntax errors.
 * Validates pipe token placement and sequences.
 */
static int	check_pipe_errors(t_token **tokens, int i, int token_count)
{
	extern int	g_exit_status;

	if (i == 0 || i == token_count - 1)
	{
		printf("minishell: syntax error near unexpected token `|'\n");
		g_exit_status = 2;
		return (1);
	}
	if (tokens[i + 1]->type == TOKEN_PIPE)
	{
		printf("minishell: syntax error near unexpected token `|'\n");
		g_exit_status = 2;
		return (1);
	}
	return (0);
}

/**
 * Checks for redirection syntax errors.
 * Validates that redirections have proper targets.
 */
static int	check_redirect_errors(t_token **tokens, int i, int token_count)
{
	extern int	g_exit_status;

	if (i == token_count - 1 || tokens[i + 1]->type != TOKEN_WORD)
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		g_exit_status = 2;
		return (1);
	}
	return (0);
}

/**
 * Checks for syntax errors in token stream.
 * Returns 1 if syntax error found, 0 if valid.
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
