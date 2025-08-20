/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:33:41 by dabierma          #+#    #+#             */
/*   Updated: 2025/08/20 15:41:12 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H
# include "shell.h"

/**
 * builtin_cd.c - Change directory builtin implementation.
 */
int			builtin_cd(char **args, char ***envp);

/**
 * builtins.c - Basic builtin command implementations.
 */
int			builtin_echo(char **args);
int			builtin_pwd(void);
int			builtin_exit(char **args);
void		env_print(char **env);

/**
 * builtins_env.c - Environment-related builtin commands.
 */
int			builtin_export(char **args, char ***envp);
int			builtin_unset(char **args, char ***envp);
int			builtin_env(char **env);

/**
 * cmd_array_utils.c - Command array manipulation utilities.
 */
int			count_args(char **args);
void		copy_replacements(char **new_cmd, int *new_i,
				char **replacements, int replacement_count);
void		copy_single_arg(char **new_cmd, int *new_i, char *old_arg);

/**
 * env.c - Environment variable access and management.
 */
char		*env_get(char **env, const char *key);
int			env_unset(char ***envp, const char *key);

/**
 * env_helper.c - Environment helper functions.
 */
char		**duplicate_env(char **envp);
void		free_env(char **env);
int			is_valid_identifier(const char *str);
int			is_assignment(const char *str);

/**
 * env_set.c - Environment variable setting functions.
 */
int			env_set(char ***envp, const char *key, const char *value);

/**
 * error.c - Error message printing functions.
 */
int			print_command_not_found(const char *cmd);
int			print_permission_denied(const char *cmd);
int			print_directory_not_found(const char *dir);

/**
 * execute.c - Main execution manager functions.
 */
int			execute_single(t_cmd_node *node, char ***envp);
int			execution_manager(t_cmd_list *cmd_list, char ***envp);

/**
 * execute_builtins.c - Builtin command execution utilities.
 */
int			is_builtin(const char *cmd);
int			exec_builtin(t_cmd_node *node, char ***envp);

/**
 * execute_expansion.c - Wildcard expansion functions.
 */
void		expand_command_args(t_cmd_node *node, char **envp);

/**
 * execute_process.c - Process execution and management.
 */
void		exec_command(t_cmd_node *node, char **env);
int			exec_external(t_cmd_node *node, char **env);
int			exec_builtin_redir(t_cmd_node *node, char ***envp);
int			handle_assignment(t_cmd_node *node, char ***envp);

/**
 * execute_redirect.c - File redirection handling.
 */
int			redir_in(t_file_node *f);
int			redir_out(t_file_node *f);
int			redir_append(t_file_node *f);
int			redir_heredoc(t_file_node *f);
int			apply_redirections(t_file_list *files, char **envp);

/**
 * execute_utils.c - General execution utility functions.
 */
void		free_split(char **arr);
char		*join_path(const char *dir, const char *cmd);
char		**get_path_dirs(char **env);
void		exec_absolute_path(t_cmd_node *node, char **env);
void		try_path_execution(t_cmd_node *node, char **env, char **paths);

/**
 * pipex.c - Pipeline execution core functions.
 */
int			count_pipeline_commands(t_cmd_node *start);
void		setup_pipes(int (*pipes)[2], int pipe_count);
t_cmd_node	*exec_pipeline(t_cmd_node *start, char ***envp);
void		close_all_pipes(int pipes[][2], int pipe_count);

/**
 * pipex_exec.c - Pipeline command execution.
 */
void		exec_middle_commands(t_cmd_node *start, pid_t *pids, int pipes[][2],
				char ***envp);
pid_t		exec_middle_cmd(t_cmd_node *node, int pipes[][2], int i,
				char ***envp);

/**
 * pipex_utils.c - Pipeline utility functions.
 */
pid_t		exec_first_cmd(t_cmd_node *node, int pipes[][2], int pipe_count,
				char ***envp);
pid_t		exec_last_cmd(t_cmd_node *node, int pipes[][2], int pipe_count,
				char ***envp);
void		wait_for_pipeline(pid_t *pids, int cmd_count);
t_cmd_node	*execute_pipeline_processes(t_cmd_node *start,
				int cmd_count, int pipe_count, char ***envp);

/**
 * redirect_utils.c
 */
char		*prepare_redirection_filename(t_file_node *f, char **envp,
				char **original_filename);
void		restore_filename(t_file_node *f, char *expanded_filename,
				char *original_filename);
int			execute_redirection(t_file_node *f);

#endif
