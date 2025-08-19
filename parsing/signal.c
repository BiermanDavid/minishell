/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:41:48 by dabierma          #+#    #+#             */
/*   Updated: 2025/08/20 00:30:51 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/**
 * Voids the signal, returns new line as it should fort ctrl+c.
 */
static void	handle_ctrl_c(int sig)
{
	(void)sig;
	write(STDERR_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/**
 * Sets minimal signal handler during command execution.
 * Prevents readline interference with child processes.
 */
void	set_minimal_signals(void)
{
	struct sigaction	sig_int;

	sigemptyset(&sig_int.sa_mask);
	sig_int.sa_handler = handle_ctrl_c_minimal;
	sig_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sig_int, NULL);
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
	struct sigaction	sig_int;
	struct sigaction	sig_quit;

	sigemptyset(&sig_int.sa_mask);
	sig_int.sa_handler = handle_ctrl_c;
	sig_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sig_int, NULL);
	sigemptyset(&sig_quit.sa_mask);
	sig_quit.sa_handler = ignore_backslash;
	sig_quit.sa_flags = SA_RESTART;
	sigaction(SIGQUIT, &sig_quit, NULL);
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
