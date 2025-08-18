/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:32:54 by dabierma          #+#    #+#             */
/*   Updated: 2025/08/18 18:06:16 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H
# include "shell.h"
# include "execute.h"

/**
 * typedef only to reduce arguments per norm (expansion quotes)
 */
typedef struct s_quote_context
{
	char	*result;
	int		*result_pos;
	char	**envp;
}	t_quote_context;

typedef struct s_exp_data
{
	char	*result;
	int		*result_pos;
}	t_exp_data;

typedef struct s_token_data
{
	t_token	**tokens;
	int		token_count;
}	t_token_data;

/**
 * Token creation and basic lexical analysis functions.
 */
t_token		*create_token(t_token_type type, const char *value, int position);
t_token		**tokenize_input(const char *input, int *j);
void		destroy_token(t_token *token);
void		*safe_realloc(void *ptr, size_t size);

/**
 * parse.c - parsing
 */
t_cmd_list	*parse_command(t_token **token, int j, char **env);

int			count_all_command_words(t_token **tokens, int start,
				int token_count);
char		**extract_all_command_args(t_token **tokens, int start, int end,
				char **envp);
int			find_command_end(t_token **tokens, int start, int token_count);

/**
 * parser_cmd.c - Command parsing and command list creation functions.
 */
void		add_cmd_to_list(t_cmd_list *list, t_cmd_node *node);
char		**allocate_args_array(int word_count);
char		*process_token_arg(t_token *token, char **envp);
char		**extract_command_args(t_token **token, int start,
				int word_count, char **envp);

/**
 * Variable expansion and quote processing for shell input.
 */
char		*expand_variables(const char *input, char **envp);
char		*process_single_quotes(const char *input);
char		*process_double_quotes(const char *input, char **envp);
char		*process_quoted_string(const char *input, char **envp);
char		*expand_token(const char *token_value, char **envp);

/**
 * expansion_utils.c - Helper functions for variable expansion.
 */
int			extract_var_name(const char *input, int input_pos, char *var_name);
int			process_special_variable(const char *input, int input_pos,
				t_exp_data *data);
int			process_escape_sequence(const char *input, int input_pos,
				char *result, int *result_pos);

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
 * heredoc_utils.c - Helper functions for heredoc processing.
 */
char		*read_heredoc(char *buffer, size_t buffer_size);
char		*realloc_heredoc_buffer(char *data, size_t *size,
				size_t needed_size, size_t current_len);
char		*init_heredoc_buffer(size_t *size, size_t *len);

/**
 * free_errors.c
 */
void		free_cmd_list(t_cmd_list *list);
void		skip_whitespace(const char *input, int *pos);

/**
 * expansion_quotes.c
 */
int			copy_var_value(char *result, int result_pos, const char *var_value);
void		remove_quotes(char *result, const char *input, int len);
char		*process_single_quotes(const char *input);

/**
 * handle_inputs_heredoc.c
 */
char		*extract_heredoc_delimiter(const char *input);
char		*read_heredoc_content(const char *delimiter);

/**
 * handle_inputs_reader.c
 */
char		*read_from_tty(void);
char		*read_from_pipe(void);

/**
 * parse_syntax.c
 */
int			check_syntax_errors(t_token **tokens, int token_count);
int			process_quoted_part(const char *input, int pos,
				t_quote_context *ctx);
int			copy_unquoted_section_expanded(const char *input, int start,
				t_quote_context *ctx);
int			has_mixed_quotes(const char *input);
char		*process_mixed_content(const char *input, char **envp);
int			process_single_command(t_token_data *data, int i,
				t_cmd_list *cmd_list, char **env);

#endif