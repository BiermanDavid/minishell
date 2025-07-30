#include "parse.h"

/**
 * Creates a new file node for redirections.
 * Allocates and initializes a file node structure.
 */
t_file_node	*create_file_node(char *filename, int redir_type)
{
	t_file_node	*node;

	node = malloc(sizeof(t_file_node));
	if (!node)
		return (NULL);
	node->filename = malloc(strlen(filename) + 1);
	if (!node->filename)
	{
		free(node);
		return (NULL);
	}
	strcpy(node->filename, filename);
	node->redir_type = redir_type;
	node->next = NULL;
	return (node);
}

/**
 * Creates a new empty file list.
 * Initializes the file list structure.
 */
t_file_list	*create_file_list(void)
{
	t_file_list	*list;

	list = malloc(sizeof(t_file_list));
	if (!list)
		return (NULL);
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
	return (list);
}

/**
 * Adds a file node to the file list.
 * Appends to the end of the list.
 */
void	add_file_to_list(t_file_list *list, t_file_node *node)
{
	if (!list || !node)
		return ;
	if (!list->head)
	{
		list->head = node;
		list->tail = node;
	}
	else
	{
		list->tail->next = node;
		list->tail = node;
	}
	list->size++;
}

/**
 * Creates a new command node.
 * Allocates and initializes a command node structure.
 */
t_cmd_node	*create_cmd_node(int cmd_type)
{
	t_cmd_node	*node;

	node = malloc(sizeof(t_cmd_node));
	if (!node)
		return (NULL);
	node->cmd_type = cmd_type;
	node->cmd = NULL;
	node->files = create_file_list();
	node->next = NULL;
	return (node);
}

/**
 * Creates a new command list.
 * Initializes the command list structure.
 */
t_cmd_list	*create_cmd_list(void)
{
	t_cmd_list	*list;

	list = malloc(sizeof(t_cmd_list));
	if (!list)
		return (NULL);
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
	return (list);
}

/**
 * Adds a command node to the command list.
 * Appends to the end of the list.
 */
void	add_cmd_to_list(t_cmd_list *list, t_cmd_node *node)
{
	if (!list || !node)
		return ;
	if (!list->head)
	{
		list->head = node;
		list->tail = node;
	}
	else
	{
		list->tail->next = node;
		list->tail = node;
	}
	list->size++;
}

/**
 * Counts WORD tokens until hitting an operator or EOF.
 * Used to determine command argument count.
 */
int	count_words_until_operator(t_token **tokens, int start, int token_count)
{
	int	count;
	int	i;

	count = 0;
	i = start;
	while (i < token_count && tokens[i]->type == TOKEN_WORD)
	{
		count++;
		i++;
	}
	return (count);
}

/**
 * Allocates memory for command arguments array.
 * Helper function for extract_command_args.
 */
char	**allocate_args_array(int word_count)
{
	char	**args;

	args = malloc((word_count + 1) * sizeof(char*));
	if (!args)
		return (NULL);
	return (args);
}

/**
 * Processes a single token for command arguments.
 * Expands token if needed and creates string copy.
 */
char	*process_token_arg(t_token *token)
{
	char	*expanded;
	char	*result;

	expanded = expand_token(token->value);
	if (expanded)
		return (expanded);
	result = malloc(strlen(token->value) + 1);
	if (result)
		strcpy(result, token->value);
	return (result);
}

/**
 * Extracts command arguments from tokens.
 * Creates the args array for a command node.
 */
char	**extract_command_args(t_token **tokens, int start, int word_count)
{
	char	**args;
	int		i;

	args = allocate_args_array(word_count);
	if (!args)
		return (NULL);
	i = 0;
	while (i < word_count)
	{
		args[i] = process_token_arg(tokens[start + i]);
		i++;
	}
	args[word_count] = NULL;
	return (args);
}

/**
 * Reads one line of heredoc input.
 * Returns processed line without trailing newline.
 */
char	*read_heredoc_line(char *buffer, size_t buffer_size)
{
	char	*line;
	size_t	line_len;

	printf("heredoc> ");
	fflush(stdout);
	if (!fgets(buffer, buffer_size, stdin))
		return (NULL);
	line = buffer;
	line_len = strlen(line);
	if (line_len > 0 && line[line_len - 1] == '\n')
	{
		line[line_len - 1] = '\0';
		line_len--;
	}
	return (line);
}

/**
 * Appends a line to heredoc content buffer.
 * Handles buffer reallocation if needed.
 */
char	*append_heredoc_line(char *content, char *line, size_t *content_len,
		size_t *content_size)
{
	size_t	line_len;

	line_len = strlen(line);
	line[line_len] = '\n';
	line_len++;
	if (*content_len + line_len + 1 > *content_size)
	{
		*content_size *= 2;
		content = realloc(content, *content_size);
		if (!content)
			return (NULL);
	}
	strcpy(content + *content_len, line);
	*content_len += line_len;
	return (content);
}

/**
 * Collects heredoc content from input until delimiter is found.
 * Returns the collected content as a single string.
 */
char	*collect_heredoc_content(const char *delimiter)
{
	char	*content;
	char	*line;
	size_t	content_size;
	size_t	content_len;
	char	buffer[1024];

	content_size = 1024;
	content = malloc(content_size);
	if (!content)
		return (NULL);
	content[0] = '\0';
	content_len = 0;
	while (1)
	{
		line = read_heredoc_line(buffer, sizeof(buffer));
		if (!line)
			break ;
		if (strcmp(line, delimiter) == 0)
			break ;
		content = append_heredoc_line(content, line, &content_len,
				&content_size);
		if (!content)
			return (NULL);
	}
	return (content);
}

/**
 * Determines redirection type from token type.
 * Maps token types to redirection types.
 */
int	get_redir_type(t_token_type token_type)
{
	if (token_type == TOKEN_REDIRECT_IN)
		return (REDIR_IN);
	else if (token_type == TOKEN_REDIRECT_OUT)
		return (REDIR_OUT);
	else if (token_type == TOKEN_REDIRECT_APPEND)
		return (REDIR_APPEND);
	else if (token_type == TOKEN_HEREDOC)
		return (REDIR_HEREDOC);
	return (0);
}

/**
 * Handles heredoc redirection parsing.
 * Collects heredoc content and creates file node.
 */
int	handle_heredoc_redir(t_token **tokens, int i, int token_count,
		t_cmd_node *cmd)
{
	t_file_node	*file_node;
	char		*heredoc_content;
	char		*content_to_use;

	i++;
	if (i >= token_count || tokens[i]->type != TOKEN_WORD)
		return (i);
	heredoc_content = collect_heredoc_content(tokens[i]->value);
	if (heredoc_content)
		content_to_use = heredoc_content;
	else
		content_to_use = "";
	file_node = create_file_node(content_to_use, REDIR_HEREDOC);
	if (file_node)
		add_file_to_list(cmd->files, file_node);
	if (heredoc_content)
		free(heredoc_content);
	return (i + 1);
}

/**
 * Handles standard redirection parsing.
 * Creates file node for input, output, or append redirections.
 */
int	handle_standard_redir(t_token **tokens, int i, int token_count,
		t_cmd_node *cmd)
{
	int			redir_type;
	t_file_node	*file_node;

	redir_type = get_redir_type(tokens[i]->type);
	i++;
	if (i >= token_count || tokens[i]->type != TOKEN_WORD)
		return (i);
	file_node = create_file_node(tokens[i]->value, redir_type);
	if (file_node)
		add_file_to_list(cmd->files, file_node);
	return (i + 1);
}

/**
 * Enhanced redirection parser that handles heredoc content.
 * Collects heredoc content when << operator is encountered.
 */
int	parse_redirections(t_token **tokens, int start, int token_count,
		t_cmd_node *cmd)
{
	int	i;

	i = start;
	while (i < token_count)
	{
		if (tokens[i]->type == TOKEN_HEREDOC)
			i = handle_heredoc_redir(tokens, i, token_count, cmd);
		else if (tokens[i]->type == TOKEN_REDIRECT_IN
			|| tokens[i]->type == TOKEN_REDIRECT_OUT
			|| tokens[i]->type == TOKEN_REDIRECT_APPEND)
			i = handle_standard_redir(tokens, i, token_count, cmd);
		else
			break ;
	}
	return (i);
}

/**
 * Determines command type based on following token.
 * Sets appropriate command type for execution.
 */
void	set_command_type(t_cmd_node *cmd, t_token **tokens, int i,
		int token_count)
{
	if (i < token_count)
	{
		if (tokens[i]->type == TOKEN_PIPE)
			cmd->cmd_type = CMD_PIPE;
		else if (tokens[i]->type == TOKEN_BACKGROUND)
			cmd->cmd_type = CMD_BACKGROUND;
	}
}

/**
 * Skips separator tokens in token array.
 * Returns updated index after skipping separators.
 */
int	skip_separators(t_token **tokens, int i, int token_count)
{
	while (i < token_count && (tokens[i]->type == TOKEN_SEMICOLON
			|| tokens[i]->type == TOKEN_BACKGROUND))
		i++;
	return (i);
}

/**
 * Processes a single command from tokens.
 * Creates command node and parses arguments and redirections.
 */
int	process_single_command(t_token **tokens, int i, int token_count,
		t_cmd_list *cmd_list)
{
	t_cmd_node	*current_cmd;
	int			word_count;

	if (tokens[i]->type != TOKEN_WORD)
		return (i + 1);
	word_count = count_words_until_operator(tokens, i, token_count);
	current_cmd = create_cmd_node(CMD_SIMPLE);
	if (!current_cmd)
		return (token_count);
	current_cmd->cmd = extract_command_args(tokens, i, word_count);
	i += word_count;
	i = parse_redirections(tokens, i, token_count, current_cmd);
	set_command_type(current_cmd, tokens, i, token_count);
	add_cmd_to_list(cmd_list, current_cmd);
	if (i < token_count && (tokens[i]->type == TOKEN_PIPE
			|| tokens[i]->type == TOKEN_BACKGROUND
			|| tokens[i]->type == TOKEN_SEMICOLON))
		i++;
	return (i);
}

/**
 * Main parser - creates clear pipe chains for execution.
 * CMD_PIPE means "pipes to next command", CMD_PIPE_END means "last in chain".
 */
t_cmd_list	*parse_command(t_token **tokens, int token_count)
{
	t_cmd_list	*cmd_list;
	int			i;

	if (!tokens || token_count == 0)
		return (NULL);
	cmd_list = create_cmd_list();
	if (!cmd_list)
		return (NULL);
	i = 0;
	while (i < token_count && tokens[i]->type != TOKEN_EOF)
	{
		i = skip_separators(tokens, i, token_count);
		if (i >= token_count)
			break ;
		i = process_single_command(tokens, i, token_count, cmd_list);
	}
	return (cmd_list);
}

/**
 * Frees a file list and all its nodes.
 * Cleans up memory for redirections.
 */
void	free_file_list(t_file_list *list)
{
	t_file_node	*current;
	t_file_node	*next;

	if (!list)
		return ;
	current = list->head;
	while (current)
	{
		next = current->next;
		free(current->filename);
		free(current);
		current = next;
	}
	free(list);
}

/**
 * Frees command arguments array.
 * Helper function for free_cmd_list.
 */
void	free_cmd_args(char **cmd)
{
	int	i;

	if (!cmd)
		return ;
	i = 0;
	while (cmd[i])
	{
		free(cmd[i]);
		i++;
	}
	free(cmd);
}

/**
 * Frees a command list and all its nodes.
 * Cleans up all allocated memory.
 */
void	free_cmd_list(t_cmd_list *list)
{
	t_cmd_node	*current;
	t_cmd_node	*next;

	if (!list)
		return ;
	current = list->head;
	while (current)
	{
		next = current->next;
		free_cmd_args(current->cmd);
		free_file_list(current->files);
		free(current);
		current = next;
	}
	free(list);
}
