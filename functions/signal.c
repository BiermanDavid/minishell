/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:41:48 by dabierma          #+#    #+#             */
/*   Updated: 2025/07/30 15:41:49 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

/**
 * Handles SIGINT (Ctrl+C) in the parent shell.
 * Displays new prompt on new line when received.
 */
void	handle_sigint_parent(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/**
 * Handles SIGQUIT (Ctrl+\) in the parent shell.
 * Does nothing as per requirements.
 */
void	handle_sigquit_parent(int sig)
{
	(void)sig;
}

/**
 * Sets up signal handlers for the parent shell.
 * Parent ignores SIGQUIT and handles SIGINT specially.
 */
void	setup_parent_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_handler = handle_sigint_parent;
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_handler = handle_sigquit_parent;
	sa_quit.sa_flags = SA_RESTART;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

/**
 * Resets signal handlers to default for child processes.
 * Children should respond normally to signals.
 */
void	setup_child_signals(void)
{
	struct sigaction	sa_default;

	sigemptyset(&sa_default.sa_mask);
	sa_default.sa_handler = SIG_DFL;
	sa_default.sa_flags = 0;
	sigaction(SIGINT, &sa_default, NULL);
	sigaction(SIGQUIT, &sa_default, NULL);
}

/**
 * Handles special readline behaviors for signals.
 * Called before readline to ensure proper signal handling.
 */
void	prepare_readline_signals(void)
{
	rl_catch_signals = 0;
}
