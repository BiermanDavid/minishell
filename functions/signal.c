/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:41:48 by dabierma          #+#    #+#             */
/*   Updated: 2025/08/04 22:05:16 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/**
 * Voids the signal, returns new line as it should fort ctrl+c.
 */
void	handle_ctrl_c(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/**
 * does basically nothing as it should. ctrl+\ is voided.
 */
void	ignore_backslash(int sig)
{
	(void)sig;
}

/**
 * Sets up kernel signals on startup, asynchronously whenever the signals arrive.
 * 		Basically just rules for operating our terminal at the baseline.
 * SIGINT handles ctrl+c printing of new line, clears prompt and uses SA_RESTART
 * 		to interrupt any active system calls.
 * SIGQUIT handles ctrl+\ by voiding the signal and doing nothing. Also
 * 		ends any syscalls and prevents core dumping or shell quitting.
 */
void	initialize_shell_signals(void)
{
	struct sigaction	interrupt_signal;
	struct sigaction	quit_signal_action;

	sigemptyset(&interrupt_signal.sa_mask);
	interrupt_signal.sa_handler = handle_ctrl_c;
	interrupt_signal.sa_flags = SA_RESTART;
	sigaction(SIGINT, &interrupt_signal, NULL);
	sigemptyset(&quit_signal_action.sa_mask);
	quit_signal_action.sa_handler = ignore_backslash;
	quit_signal_action.sa_flags = SA_RESTART;
	sigaction(SIGQUIT, &quit_signal_action, NULL);
}

/**
 * Clears blocked signals and restores SIGINT+SIGQUIT to default
 * allows for forks and execs to run processes just like in the real shell.
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
void	ignore_rl_sigint_and_sigquit(void)
{
	rl_catch_signals = 0;
}
