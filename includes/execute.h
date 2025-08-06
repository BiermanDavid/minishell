/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:33:41 by dabierma          #+#    #+#             */
/*   Updated: 2025/08/04 21:43:42 by dgessner         ###   ########.fr       */
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
int			apply_redirections(t_file_list *files);

/* Pipeline execution */
pid_t		spawn_stage(t_cmd_node *node, int fds[2],
				t_pipe_info *pipe_info, char ***envp);
t_cmd_node	*exec_pipeline(t_cmd_node *start, char ***envp);

/* Command validation functions */
bool		validate_cat_args(char **args);
bool		validate_mkdir_args(char **args);
bool		validate_ls_args(char **args);
bool		validate_grep_args(char **args);
bool		is_cat_command(const char *cmd);
bool		is_mkdir_command(const char *cmd);
bool		is_ls_command(const char *cmd);
bool		is_grep_command(const char *cmd);

#endif
