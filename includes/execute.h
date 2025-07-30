/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:33:41 by dabierma          #+#    #+#             */
/*   Updated: 2025/07/30 16:33:48 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H
# include "shell.h"

/**
 * execute.c
 * Simple execution manager for parsed commands.
 */
int		execution_manager(t_cmd_list *cmd_list);
void	print_parsed_commands(t_cmd_list *cmd_list);

#endif

// echo "Hello from minishell!" > output.txt
// echo "Hello from minishell!" > output.txt
// cat < output.txt
// cat output.txt | grep "minishell"
// echo "Second line" >> output.txt
// echo "Background task" &
