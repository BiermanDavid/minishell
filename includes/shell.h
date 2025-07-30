/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:33:58 by dabierma          #+#    #+#             */
/*   Updated: 2025/07/30 19:38:41 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>
# include <sys/types.h>
# include <unistd.h>
# include <ctype.h>

typedef struct s_file_node	t_file_node;
typedef struct s_cmd_node	t_cmd_node;
typedef struct s_file_list	t_file_list;
typedef struct s_cmd_list	t_cmd_list;

/**
 * Token types produced by the lexer for different shell elements.
 */
typedef enum t_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_APPEND,
	TOKEN_HEREDOC,
	TOKEN_LOGICAL_AND,
	TOKEN_LOGICAL_OR,
	TOKEN_SEMICOLON,
	TOKEN_BACKGROUND,
	TOKEN_EOF
}	t_token_type;

/**
 * Redirection types for file input and output operations.
 */
typedef enum t_redir_type
{
	REDIR_IN = 1,
	REDIR_OUT = 2,
	REDIR_APPEND = 3,
	REDIR_HEREDOC = 4
}	t_redir_type;

/**
 * Command types for different shell command structures.
 */
typedef enum t_cmd_type
{
	CMD_SIMPLE,
	CMD_PIPE,
	CMD_BUILTIN,
	CMD_EXTERNAL,
	CMD_LOGICAL_AND,
	CMD_LOGICAL_OR,
	CMD_SEQUENCE,
	CMD_BACKGROUND
}	t_cmd_type;

typedef struct t_token
{
	t_token_type	type;
	char			*value;
	int				position;
}	t_token;

typedef struct s_file_node
{
	char					*filename;
	int						redir_type;
	struct s_file_node		*next;
}	t_file_node;

typedef struct s_file_list
{
	t_file_node	*head;
	t_file_node	*tail;
	ssize_t		size;
}	t_file_list;

typedef struct s_cmd_node
{
	int					cmd_type;
	char				**cmd;
	t_file_list			*files;
	struct s_cmd_node	*next;
}	t_cmd_node;

typedef struct s_cmd_list
{
	t_cmd_node	*head;
	t_cmd_node	*tail;
	ssize_t		size;
}	t_cmd_list;

/**
 * Shell loop and user interface functions for input handling.
 */
void	display_prompt(void);
void	strip_newline(char *input);
void	cleanup_tokens(t_token **tokens, int token_count);
void	run_shell_loop(void);

/**
 * Signal.c - Signal handling functions for readline.
 */
void	setup_parent_signals(void);
void	setup_child_signals(void);
void	prepare_readline_signals(void);
void	handle_sigint_parent(int sig);
void	handle_sigquit_parent(int sig);

/**
 * handle_inputs.c - manages response for input edge cases
 */
char	*read_input(void);
bool	is_empty_line(const char *input);
bool	handle_exit_command(char *input);
void	process_input(char *input);

#endif