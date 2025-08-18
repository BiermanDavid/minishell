/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 18:33:14 by dabierma          #+#    #+#             */
/*   Updated: 2025/08/06 18:34:15 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/**
 * Configures readline signal behavior.
 * Prevents readline from catching signals itself.
 */
void	ignore_rl_sigint_and_sigquit(void)
{
	rl_catch_signals = 0;
}

/**
 * Minimal signal handler for during command execution.
 * Just prints newline, no readline management.
 */
void	handle_ctrl_c_minimal(int sig)
{
	(void)sig;
	write(STDERR_FILENO, "\n", 1);
}

/**
 * does basically nothing as it should. ctrl+\ is voided.
 */
void	ignore_backslash(int sig)
{
	(void)sig;
}
