/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:33:41 by dabierma          #+#    #+#             */
/*   Updated: 2025/08/06 18:40:54 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H
# include "shell.h"

/* Execution manager */
int			execution_manager(t_cmd_list *cmd_list, char ***envp);
void		exec_command(t_cmd_node *node, char **env);
int			exec_external(t_cmd_node *node, char **env);
int			exec_builtin_redir(t_cmd_node *node, char ***envp);
int			execute_single(t_cmd_node *node, char ***envp);
int			handle_assignment(t_cmd_node *node, char ***envp);
int			is_variable_assignment(t_cmd_node *node);

/* Utility functions */
void		free_split(char **arr);
char		*join_path(const char *dir, const char *cmd);
char		**get_path_dirs(char **env);
void		exec_absolute_path(t_cmd_node *node, char **env);
void		try_path_execution(t_cmd_node *node, char **env, char **paths);
int			print_permission_denied(const char *cmd);
int			print_command_not_found(const char *cmd);

/* Builtin utilities */
int			builtin_echo(char **args);
int			builtin_pwd(void);
int			builtin_exit(char **args);
int			is_builtin(const char *cmd);
int			is_valid_identifier(const char *str);
int			is_assignment(const char *str);
int			exec_builtin(t_cmd_node *node, char ***envp);
int			builtin_export(char **args, char ***envp);
int			builtin_unset(char **args, char ***envp);
int			builtin_cd(char **args, char ***envp);
int			builtin_env(char **env);

/* Environment helpers */
char		*env_get(char **env, const char *key);
int			env_set(char ***envp, const char *key, const char *value);
int			env_unset(char ***envp, const char *key);
void		env_print(char **env);
char		**duplicate_env(char **envp);
void		free_env(char **env);

/* Redirection utilities */
int			redir_in(t_file_node *f);
int			redir_out(t_file_node *f);
int			redir_append(t_file_node *f);
int			redir_heredoc(t_file_node *f);
int			apply_redirections(t_file_list *files, char **envp);
char		*prepare_redirection_filename(t_file_node *f, char **envp,
				char **original_filename);
void		restore_filename(t_file_node *f, char *expanded_filename,
				char *original_filename);
int			execute_redirection(t_file_node *f);

/* Pipeline execution */
pid_t		spawn_stage(t_cmd_node *node, int fds[2],
				t_pipe_info *pipe_info, char ***envp);
t_cmd_node	*exec_pipeline(t_cmd_node *start, char ***envp);
int			count_pipeline_commands(t_cmd_node *start);
void		setup_pipes(int (*pipes)[2], int pipe_count);
pid_t		exec_first_cmd(t_cmd_node *node, int (*pipes)[2], char ***envp);
pid_t		exec_middle_cmd(t_cmd_node *node, int pipes[][2], int i,
				char ***envp);
pid_t		exec_last_cmd(t_cmd_node *node, int pipes[][2], int pipe_count,
				char ***envp);
void		set_minimal_signals(void);
t_cmd_node	*execute_pipeline_processes(t_cmd_node *start,
				int cmd_count, int pipe_count, char ***envp);
void		exec_middle_commands(t_cmd_node *start, pid_t *pids, int pipes[][2],
				char ***envp);
void		wait_for_pipeline(pid_t *pids, int cmd_count);
void		restore_shell_signals(void);
void		handle_ctrl_c_minimal(int sig);
void		close_all_pipes(int (*pipes)[2], int pipe_count);

/* Command validation functions */
bool		validate_cat_args(char **args);
bool		validate_mkdir_args(char **args);
bool		validate_ls_args(char **args);
bool		validate_grep_args(char **args);
bool		is_cat_command(const char *cmd);
bool		is_mkdir_command(const char *cmd);
bool		is_ls_command(const char *cmd);
bool		is_grep_command(const char *cmd);

/* Wildcard expansion functions */
char		**expand_wildcards(const char *pattern, int *match_count);
void		expand_command_args(t_cmd_node *node, char **envp);
int			wildcard_match(const char *pattern, const char *str);
char		**create_match_array(char **matches, int count);
char		**resize_matches(char **old_matches, int old_capacity,
				int new_capacity);
int			count_args(char **args);
void		copy_replacements(char **new_cmd, int *new_i,
				char **replacements, int replacement_count);
void		copy_single_arg(char **new_cmd, int *new_i, char *old_arg);

#endif
