/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:29:55 by dabierma          #+#    #+#             */
/*   Updated: 2025/08/02 19:35:23 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"
#include "execute.h"
#include <readline/readline.h>
#include <readline/history.h>

/**
 * Removes the newline character from input string.
 * Not needed with readline but kept for compatibility.
 */
void	strip_newline(char *input)
{
	int	len;

	len = strlen(input);
	if (len > 0 && input[len - 1] == '\n')
		input[len - 1] = '\0';
}

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
 * Initializes shell environment (signal.c)
 * Sets up signals and displays welcome message.
 */
void	initialize_shell(void)
{
	printf("Welcome to minishell!\n");
	initialize_shell_signals();
	ignore_rl_sigint_and_sigquit();
}

/**
 * Main shell loop that handles the read-eval-print cycle.
 * Continues until user exits or EOF is encountered.
 */
int	main(void)
{
	char	*input;

	initialize_shell();
	while (true)
	{
		input = read_input();
		if (!input)
		{
			// printf("Goodbye!\n"); // Maybe add perror
			break ;
		}
		if (is_empty_line(input))
		{
			free(input);
			continue ;
		}
		process_input(input);
		free(input);
	}
	return (0);
}

