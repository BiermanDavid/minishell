/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:33:58 by dabierma          #+#    #+#             */
/*   Updated: 2025/08/20 04:45:09 by dabierma         ###   ########.fr       */
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
# include <limits.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <ctype.h>
# include <signal.h>
# include "../libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <dirent.h>
# include <sys/stat.h>

extern int		g_exit_status;

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
	CMD_BACKGROUND
}	t_cmd_type;

typedef struct s_pipe_info
{
	int	pipes[64][2];
	int	total;
}	t_pipe_info;

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
 * Signal.c - Signal handling functions for readline.
 */
void	initialize_shell_signals(void);
void	setup_child_signals(void);
void	set_minimal_signals(void);

/**
 * signal_utils.c - Wrapper functions for signal handling.
 */
void	ignore_rl_sigint_and_sigquit(void);
void	handle_ctrl_c_minimal(int sig);
void	ignore_backslash(int sig);

/**
 * handle_inputs.c - manages response for input edge cases
 */
int		process_input(char *input, char ***envp);
char	*read_input(void);
bool	is_empty_line(const char *input);

/**
 * memory_utils.c - standardized error handling
 */
void	*safe_malloc(size_t size);
char	*safe_strdup(const char *s);
void	*safe_realloc(void *ptr, size_t old_size, size_t new_size);
void	destroy_token(t_token *token);

/**
 * wildcard_expansion.c - 
 */
char	**expand_wildcards(const char *pattern, int *match_count);

/**
 * wildcard_utils.c -
 */
int		wildcard_match(const char *pattern, const char *str);
char	**create_match_array(char **matches, int count);
char	**resize_matches(char **old_matches, int old_capacity,
			int new_capacity);

#endif