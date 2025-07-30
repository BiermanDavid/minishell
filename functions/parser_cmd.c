/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:43:19 by dabierma          #+#    #+#             */
/*   Updated: 2025/07/30 16:32:38 by dabierma         ###   ########.fr       */
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
