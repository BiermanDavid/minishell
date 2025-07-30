#ifndef PARSE_H
# define PARSE_H
# include <ctype.h>

/**
 * Token creation and basic lexical analysis functions.
 */
t_token		*create_token(t_token_type type, const char *value, int position);
t_token		**tokenize_input(const char *input, int *token_count);
void		destroy_token(t_token *token);

/**
 * Command parsing and command list creation functions.
 */
t_cmd_list	*parse_command(t_token **tokens, int token_count);
void		free_cmd_list(t_cmd_list *list);
char		*collect_heredoc_content(const char *delimiter);

/**
 * Variable expansion and quote processing for shell input.
 */
char		*expand_variables(const char *input);
char		*process_single_quotes(const char *input);
char		*process_double_quotes(const char *input);
char		*process_quoted_string(const char *input);
char		*expand_token(const char *token_value);

#endif