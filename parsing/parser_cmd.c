/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:43:19 by dabierma          #+#    #+#             */
/*   Updated: 2025/08/19 22:58:02 by dabierma         ###   ########.fr       */
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
static char	**allocate_args_array(int word_count)
{
	char	**args;

	args = safe_malloc((word_count + 1) * sizeof(char *));
	if (!args)
		return (NULL);
	return (args);
}

/**
 * Processes a single token for command arguments.
 * Expands token if needed and creates string copy.
 */
static char	*process_token_arg(t_token *token, char **envp)
{
	char	*result;

	(void)envp;
	result = safe_strdup(token->value);
	return (result);
}

/**
 * Extracts command arguments from tokens.
 * Creates the args array for a command node.
 */
char	**extract_command_args(t_token **tokens, int start,
		int word_count, char **envp)
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
		if (!args[i])
		{
			while (--i >= 0)
				free(args[i]);
			free(args);
			return (NULL);
		}
		i++;
	}
	args[word_count] = NULL;
	return (args);
}
