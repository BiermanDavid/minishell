#include "parse.h"
#include "execute.h"
#include <readline/readline.h>
#include <readline/history.h>

/**
 * Reads a line of input from the user.
 * Uses readline for enhanced input with history support.
 */
char	*read_input(void)
{
	char	*input;

	input = readline("minishell> ");
	if (input && *input)
		add_history(input);
	return (input);
}

/**
 * Removes the newline character from input string.
 * Not needed with readline but kept for compatibility.
 */
void	strip_newline(char *input)
{
	int	len;

	len = strlen(input);
	if (len > 0 && input[len - 1] == '\n')
		input[len - 1] = '\0';
}

/**
 * Checks if the input line is empty or only whitespace.
 * Returns true if line should be ignored.
 */
bool	is_empty_line(const char *input)
{
	int	i;

	if (!input)
		return (true);
	i = 0;
	while (input[i])
	{
		if (input[i] != ' ' && input[i] != '\t')
			return (false);
		i++;
	}
	return (true);
}

/**
 * Cleanup function for token array.
 * Frees all tokens and the array itself.
 */
void	cleanup_tokens(t_token **tokens, int token_count)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (i < token_count)
	{
		destroy_token(tokens[i]);
		i++;
	}
	free(tokens);
}

/**
 * Handles exit command processing.
 * Returns true if shell should exit.
 */
bool	handle_exit_command(char *input)
{
	if (strcmp(input, "exit") == 0)
	{
		free(input);
		printf("Goodbye!\n");
		return (true);
	}
	return (false);
}

/**
 * Processes parsed command list.
 * Handles execution without debug output.
 */
void	process_command_list(t_cmd_list *cmd_list)
{
	if (cmd_list && cmd_list->size > 0)
	{
		execution_manager(cmd_list);
		free_cmd_list(cmd_list);
	}
}

/**
 * Processes user input through tokenization and parsing.
 * Handles the complete input processing pipeline.
 */
void	process_input(char *input)
{
	t_token		**tokens;
	int			token_count;
	t_cmd_list	*cmd_list;

	tokens = tokenize_input(input, &token_count);
	if (tokens && token_count > 0)
	{
		cmd_list = parse_command(tokens, token_count);
		process_command_list(cmd_list);
		cleanup_tokens(tokens, token_count);
	}
}

/**
 * Initializes shell environment.
 * Sets up signals and displays welcome message.
 */
void	initialize_shell(void)
{
	printf("Welcome to minishell!\n");
	setup_parent_signals();
	prepare_readline_signals();
}

/**
 * Main shell loop that handles the read-eval-print cycle.
 * Continues until user exits or EOF is encountered.
 */
void	run_shell_loop(void)
{
	char	*input;

	initialize_shell();
	while (true)
	{
		input = read_input();
		if (!input)
		{
			printf("\nGoodbye!\n");
			break ;
		}
		if (is_empty_line(input))
		{
			free(input);
			continue ;
		}
		if (handle_exit_command(input))
			break ;
		process_input(input);
		free(input);
	}
}

/**
 * Program entry point.
 * Initializes the shell and starts the main loop.
 */
int	main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	run_shell_loop();
	return (0);
}
