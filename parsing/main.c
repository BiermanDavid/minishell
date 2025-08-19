/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:29:55 by dabierma          #+#    #+#             */
/*   Updated: 2025/08/19 19:36:48 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"
#include "execute.h"

/**
 * Cleanup function for token array.
 * Frees all tokens and the array itself.
 */
void	cleanup_tokens(t_token **tokens, int token_count)
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
 * Initializes shell environment signals
 */
void	initialize_shell(void)
{
	initialize_shell_signals();
	ignore_rl_sigint_and_sigquit();
}

/**
 * Main shell loop, set up env and check for inputs
 * func call for empty line edge case
 * process input for parsing.
 * set up an env which allows us to execute local information
 * like LS bin or USER type variabels.
 * env allows us to find paths on local cmputer
 */
int	main(int argc, char **argv, char **envp)
{
	char	*input;
	char	**new_envp;

	(void)argc;
	(void)argv;
	initialize_shell();
	new_envp = duplicate_env(envp);
	while (true)
	{
		input = read_input();
		if (!input)
			break ;
		if (is_empty_line(input))
		{
			free(input);
			continue ;
		}
		process_input(input, &new_envp);
		free(input);
	}
	free_env(new_envp);
	return (g_exit_status);
}
