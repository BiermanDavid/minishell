/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:33:41 by dabierma          #+#    #+#             */
/*   Updated: 2025/08/03 01:07:52 by dgessner         ###   ########.fr       */
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
 * builtins.c
 * Rebuildes the builtin-commands and searches 
 * for the correct commmand
 */
int		builtin_echo(char **args);
int		builtin_pwd(void);
int 	builtin_exit(char **args);

int 	is_builtin(const char *cmd);
int	exec_builtin(t_cmd_node *node, char ***envp);

int	builtin_export(char **args, char ***envp);
int	builtin_unset(char **args, char ***envp);
int	builtin_cd(char **args, char **env);
int	builtin_env(char **env);

/**
 * execute.c
 * Executes piped external commands like ls | grep.
 */
int execution_manager(t_cmd_list *cmd_list);

char	*env_get(char **env, const char *key);

/**
 * Setzt oder ersetzt eine Umgebungsvariable im env-Array.
 * Bei Erfolg wird 0 zurückgegeben, bei Fehler 1.
 */
int		env_set(char ***envp, const char *key, const char *value);

/**
 * Entfernt eine Umgebungsvariable aus dem env-Array.
 * Bei Erfolg wird 0 zurückgegeben, bei Misserfolg 1.
 */
int		env_unset(char ***envp, const char *key);

/**
 * Gibt alle Umgebungsvariablen im env-Array aus.
 */
void	env_print(char **env);

/* ========================================================================= */
/*                          Redirection Utilities                            */
/* ========================================================================= */

int redir_in(t_file_node *f);
int redir_out(t_file_node *f);
int redir_append(t_file_node *f);
int redir_heredoc(t_file_node *f);
int apply_redirections(t_file_list *files);

/* ========================================================================= */
/*                          Process Execution                                */
/* ========================================================================= */

int exec_external(t_cmd_node *node, char **env);
int	exec_builtin_redir(t_cmd_node *node, char ***envp);
int execute_single(t_cmd_node *node, char ***envp);

/* ========================================================================= */
/*                          Pipeline Execution                               */
/* ========================================================================= */

pid_t	spawn_stage(t_cmd_node *node, int in_fd, int out_fd, int pipes[64][2], int total);
t_cmd_node	*exec_pipeline(t_cmd_node *start);


/**
 * debug.c
 * Debug Utilities for parsed commands. Shows: Type, 
 * Program, Arguments and Redirections
 */
void print_parsed_commands(t_cmd_list *cmd_list);
void print_command(t_cmd_node *cmd, int index);
void print_args(char **args);
void print_cmd_type(t_cmd_node *cmd);
void print_files(t_file_node *file);

#endif

// echo "Hello from minishell!" > output.txt
// echo "Hello from minishell!" > output.txt
// cat < output.txt
// cat output.txt | grep "minishell"
// echo "Second line" >> output.txt
// echo "Background task" &
