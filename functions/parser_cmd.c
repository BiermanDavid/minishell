/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:43:19 by dabierma          #+#    #+#             */
/*   Updated: 2025/08/03 02:29:50 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

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
 * Allocates memory for command arguments array.
 * Helper function for extract_command_args.
 */
char	**allocate_args_array(int word_count)
{
	char	**args;

	args = malloc((word_count + 1) * sizeof(char *));
	if (!args)
		return (NULL);
	return (args);
}

// /**
//  * Processes a single token for command arguments.
//  * Expands token if needed and creates string copy.
//  */
// char	*process_token_arg(t_token *token)
// {
// 	char	*expanded;
// 	char	*result;

// 	expanded = expand_token(token->value);
// 	if (expanded)
// 		return (expanded);
// 	result = malloc(strlen(token->value) + 1);
// 	if (result)
// 		strcpy(result, token->value);
// 	return (result);
// }

/**
 * Processes a single token for command arguments.
 * Expands token if needed and creates string copy.
 * TAYLORS VERSION
 */
char	*process_token_arg(t_token *token, char **envp)
{
	char	*expanded;
	char	*result;

	expanded = expand_token(token->value, envp);
	if (expanded)
		return (expanded);
	result = malloc(strlen(token->value) + 1);
	if (result)
		strcpy(result, token->value);
	return (result);
}


// /**
//  * Extracts command arguments from tokens.
//  * Creates the args array for a command node.
//  */
// char	**extract_command_args(t_token **tokens, int start, int word_count)
// {
// 	char	**args;
// 	int		i;

// 	args = allocate_args_array(word_count);
// 	if (!args)
// 		return (NULL);
// 	i = 0;
// 	while (i < word_count)
// 	{
// 		args[i] = process_token_arg(tokens[start + i]);
// 		i++;
// 	}
// 	args[word_count] = NULL;
// 	return (args);
// }

/**
 * Extracts command arguments from tokens.
 * Creates the args array for a command node.
 * TAYLORS VERSION
 */
char	**extract_command_args(t_token **tokens, int start, int word_count, char **envp)
{
	char	**args;
	int		i;

	args = allocate_args_array(word_count);
	if (!args)
		return (NULL);
	i = 0;
	while (i < word_count)
	{
		args[i] = process_token_arg(tokens[start + i], envp);
		i++;
	}
	args[word_count] = NULL;
	return (args);
}
