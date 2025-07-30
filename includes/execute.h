/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:33:41 by dabierma          #+#    #+#             */
/*   Updated: 2025/07/30 19:53:54 by dgessner         ###   ########.fr       */
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

/**
 * buildins.c
 * Rebuildes the builtin-commands and searches 
 * for the correct commmand
 */
void	our_echo(t_cmd_node *cmd_node);
void	our_cd(t_cmd_node *cmd_node);
void	search_command(t_cmd_node *cmd_node);

/**
 * pipex.c
 * Executes piped external commands like ls | grep.
 */
void	run_piped_commands(t_cmd_list *cmd_list, char **envp);
char	*find_cmd(char *cmd, char **envp);
void	free_split(char **arr);

#endif

// echo "Hello from minishell!" > output.txt
// echo "Hello from minishell!" > output.txt
// cat < output.txt
// cat output.txt | grep "minishell"
// echo "Second line" >> output.txt
// echo "Background task" &
