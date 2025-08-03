/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:32:54 by dabierma          #+#    #+#             */
/*   Updated: 2025/08/03 03:10:58 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H
# include "shell.h"
# include "execute.h"

/**
 * Token creation and basic lexical analysis functions.
 */
t_token		*create_token(t_token_type type, const char *value, int position);
t_token		**tokenize_input(const char *input, int *j);
void		destroy_token(t_token *token);

/**
 * parse.c - parsing
 */
t_cmd_list	*parse_command(t_token **token, int j, char **env);

/**
 * parser_cmd.c - Command parsing and command list creation functions.
 */
void		add_cmd_to_list(t_cmd_list *list, t_cmd_node *node);
char		**allocate_args_array(int word_count);
char		*process_token_arg(t_token *token, char **envp);
char		**extract_command_args(t_token **token, int start, int word_count, char **envp);

/**
 * Variable expansion and quote processing for shell input.
 */
char		*expand_variables(const char *input, char **envp);
char		*process_single_quotes(const char *input);
char		*process_double_quotes(const char *input, char **envp);
char		*process_quoted_string(const char *input, char **envp);
char		*expand_token(const char *token_value, char **envp);

/**
 * make_parser_nodes.c -
 */
t_file_node	*create_file_node(char *filename, int redir_type);
t_file_list	*create_file_list(void);
void		add_file_to_list(t_file_list *list, t_file_node *node);
t_cmd_node	*create_cmd_node(int cmd_type);
t_cmd_list	*create_cmd_list(void);

/**
 * Lexer.c
 */

/**
 * lexer_word.c
 */
bool		is_word_delimiter(char c);
void		handle_quote(const char *input, int *pos, bool *in_q, char *q_char);
int			calculate_word_length(const char *input, int *pos);
void		copy_word(char *word, const char *input, int start, int len);
char		*read_word(const char *input, int *pos);

/**
 * Lexer_utils.c
 */
int			multi_char(const char *input, int pos, t_token **tokens, int i);
int			single_char(const char *input, int pos, t_token **tokens, int i);
int			word_token(const char *input, int pos, t_token **tokens, int i);
bool		is_multi_char_op(const char *input, int pos);
bool		is_single_char_op(char c);

/**
 * redirects.c - edge cases for handling redirections < >
 */
int			get_redir_type(t_token_type token_type);
int			heredoc_redir(t_token **token, int i, int j, t_cmd_node *cmd);
int			standard_redir(t_token **token, int i, int j, t_cmd_node *cmd);
int			parse_redir(t_token **token, int start, int j, t_cmd_node *cmd);

/**
 * heredoc.c - edge cases for handling heredocs << >>
 */
char		*collect_heredoc_content(const char *delimiter);
char		*read_heredoc_line(char *buffer, size_t buffer_size);
char		*append_heredoc(char *data, char *line, size_t *len, size_t *size);

/**
 * free_errors.c
 */
void		free_file_list(t_file_list *list);
void		free_cmd_args(char **cmd);
void		free_cmd_list(t_cmd_list *list);

/**
 * expansion_quotes.c
 */
int			copy_var_value(char *result, int result_pos, const char *var_value);
void		remove_quotes(char *result, const char *input, int len);
char		*process_single_quotes(const char *input);

#endif