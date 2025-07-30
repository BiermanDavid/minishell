/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:32:54 by dabierma          #+#    #+#             */
/*   Updated: 2025/07/30 16:32:55 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H
# include "shell.h"

/**
 * Token creation and basic lexical analysis functions.
 */
t_token		*create_token(t_token_type type, const char *value, int position);
t_token		**tokenize_input(const char *input, int *token_count);
void		destroy_token(t_token *token);

/**
 * parse.c - parsing
 */
t_cmd_list	*parse_command(t_token **tokens, int token_count);

/**
 * parser_cmd.c - Command parsing and command list creation functions.
 */
void		add_cmd_to_list(t_cmd_list *list, t_cmd_node *node);
char		**allocate_args_array(int word_count);
char		*process_token_arg(t_token *token);
char		**extract_command_args(t_token **tokens, int start, int word_count);

/**
 * Variable expansion and quote processing for shell input.
 */
char		*expand_variables(const char *input);
char		*process_single_quotes(const char *input);
char		*process_double_quotes(const char *input);
char		*process_quoted_string(const char *input);
char		*expand_token(const char *token_value);

/**
 * make_parser_nodes.c -
 */
t_file_node	*create_file_node(char *filename, int redir_type);
t_file_list	*create_file_list(void);
void		add_file_to_list(t_file_list *list, t_file_node *node);
t_cmd_node	*create_cmd_node(int cmd_type);
t_cmd_list	*create_cmd_list(void);

/**
 * redirects.c - edge cases for handling redirections < >
 */
int			get_redir_type(t_token_type token_type);
int			handle_heredoc_redir(t_token **tokens, int i, int token_count, t_cmd_node *cmd);
int			handle_standard_redir(t_token **tokens, int i, int token_count, t_cmd_node *cmd);
int			parse_redirections(t_token **tokens, int start, int token_count, t_cmd_node *cmd);

/**
 * heredoc.c - edge cases for handling heredocs << >>
 */
char		*collect_heredoc_content(const char *delimiter);
char		*read_heredoc_line(char *buffer, size_t buffer_size);
char		*append_heredoc_line(char *content, char *line, size_t *content_len, size_t *content_size);

/**
 * free_errors.c
 */
void		free_file_list(t_file_list *list);
void		free_cmd_args(char **cmd);
void		free_cmd_list(t_cmd_list *list);

#endif