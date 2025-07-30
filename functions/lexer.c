#include "parse.h"

/**
 * Skips whitespace characters in the input string.
 * Advances position until non-whitespace character is found.
 */
void	skip_whitespace(const char *input, int *pos)
{
	while (input[*pos] == ' ' || input[*pos] == '\t')
		(*pos)++;
}

/**
 * Checks if character should break word reading.
 * Returns true for delimiters and operators.
 */
bool	is_word_delimiter(char c)
{
	return (c == ' ' || c == '\t' || c == '|' || c == '&'
		|| c == '>' || c == '<' || c == ';');
}

/**
 * Handles quote processing in word reading.
 * Updates position and quote state.
 */
void	handle_quote(const char *input, int *pos, bool *in_quotes,
		char *quote_char)
{
	if (!*in_quotes)
	{
		if (input[*pos] == '"' || input[*pos] == '\'')
		{
			*quote_char = input[*pos];
			*in_quotes = true;
		}
	}
	else
	{
		if (input[*pos] == *quote_char)
		{
			*in_quotes = false;
			*quote_char = '\0';
		}
	}
}

/**
 * Calculates word length with quote handling.
 * Returns the length of the word token.
 */
int	calculate_word_length(const char *input, int *pos)
{
	int		start;
	bool	in_quotes;
	char	quote_char;

	start = *pos;
	in_quotes = false;
	quote_char = '\0';
	while (input[*pos])
	{
		handle_quote(input, pos, &in_quotes, &quote_char);
		if (!in_quotes && is_word_delimiter(input[*pos]))
			break ;
		(*pos)++;
	}
	return (*pos - start);
}

/**
 * Copies word from input to allocated buffer.
 * Helper function for read_word.
 */
void	copy_word(char *word, const char *input, int start, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		word[i] = input[start + i];
		i++;
	}
	word[len] = '\0';
}

/**
 * Reads a word token from input with proper quote handling.
 * Handles single quotes, double quotes, and bare words correctly.
 */
char	*read_word(const char *input, int *pos)
{
	int		start;
	int		len;
	char	*word;

	start = *pos;
	len = calculate_word_length(input, pos);
	word = malloc(len + 1);
	if (!word)
		return (NULL);
	copy_word(word, input, start, len);
	return (word);
}

/**
 * Creates a token with the specified type, value, and position.
 * Allocates memory and copies the value string.
 */
t_token	*create_token(t_token_type type, const char *value, int position)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->position = position;
	if (value)
	{
		token->value = malloc(strlen(value) + 1);
		if (!token->value)
		{
			free(token);
			return (NULL);
		}
		strcpy(token->value, value);
	}
	else
		token->value = NULL;
	return (token);
}

/**
 * Handles multi-character operators.
 * Returns updated position after processing operator.
 */
int	handle_multi_char_ops(const char *input, int pos, t_token **tokens,
		int count)
{
	if (input[pos] == '>' && input[pos + 1] == '>')
	{
		tokens[count] = create_token(TOKEN_REDIRECT_APPEND, ">>", pos);
		return (pos + 2);
	}
	else if (input[pos] == '<' && input[pos + 1] == '<')
	{
		tokens[count] = create_token(TOKEN_HEREDOC, "<<", pos);
		return (pos + 2);
	}
	return (pos);
}

/**
 * Handles single-character operators.
 * Returns updated position after processing operator.
 */
int	handle_single_char_ops(const char *input, int pos, t_token **tokens,
		int count)
{
	if (input[pos] == '|')
		tokens[count] = create_token(TOKEN_PIPE, "|", pos);
	else if (input[pos] == '&')
		tokens[count] = create_token(TOKEN_BACKGROUND, "&", pos);
	else if (input[pos] == '>')
		tokens[count] = create_token(TOKEN_REDIRECT_OUT, ">", pos);
	else if (input[pos] == '<')
		tokens[count] = create_token(TOKEN_REDIRECT_IN, "<", pos);
	else if (input[pos] == ';')
		tokens[count] = create_token(TOKEN_SEMICOLON, ";", pos);
	return (pos + 1);
}

/**
 * Processes a word token.
 * Returns updated position after processing word.
 */
int	process_word_token(const char *input, int pos, t_token **tokens,
		int count)
{
	char	*word;
	int		word_start;

	word_start = pos;
	word = read_word(input, &pos);
	if (word)
	{
		tokens[count] = create_token(TOKEN_WORD, word, word_start);
		free(word);
	}
	return (pos);
}

/**
 * Checks if operator is multi-character.
 * Returns true for >> and << operators.
 */
bool	is_multi_char_op(const char *input, int pos)
{
	return ((input[pos] == '>' && input[pos + 1] == '>')
		|| (input[pos] == '<' && input[pos + 1] == '<'));
}

/**
 * Checks if character is single-character operator.
 * Returns true for single character operators.
 */
bool	is_single_char_op(char c)
{
	return (c == '|' || c == '&' || c == '>' || c == '<' || c == ';');
}

/**
 * Processes single token from input.
 * Returns updated position and increments count.
 */
int	process_single_token(const char *input, int pos, t_token **tokens,
		int *count)
{
	if (is_multi_char_op(input, pos))
	{
		pos = handle_multi_char_ops(input, pos, tokens, *count);
		(*count)++;
	}
	else if (is_single_char_op(input[pos]))
	{
		pos = handle_single_char_ops(input, pos, tokens, *count);
		(*count)++;
	}
	else
	{
		pos = process_word_token(input, pos, tokens, *count);
		(*count)++;
	}
	return (pos);
}

/**
 * Simple tokenizer that handles basic shell input.
 * Recognizes words, pipes, background, and redirection operators.
 */
t_token	**tokenize_input(const char *input, int *token_count)
{
	t_token	**tokens;
	int		pos;
	int		count;

	if (!input || !token_count)
		return (NULL);
	tokens = malloc(64 * sizeof(t_token*));
	if (!tokens)
		return (NULL);
	pos = 0;
	count = 0;
	while (input[pos] != '\0' && count < 63)
	{
		skip_whitespace(input, &pos);
		if (input[pos] == '\0')
			break ;
		pos = process_single_token(input, pos, tokens, &count);
	}
	tokens[count] = create_token(TOKEN_EOF, NULL, pos);
	count++;
	*token_count = count;
	return (tokens);
}

/**
 * Frees memory for a single token.
 * Cleans up the value string and the token structure.
 */
void	destroy_token(t_token *token)
{
	if (!token)
		return ;
	if (token->value)
		free(token->value);
	free(token);
}
