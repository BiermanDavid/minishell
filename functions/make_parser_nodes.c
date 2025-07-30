/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_parser_nodes.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:57:05 by dabierma          #+#    #+#             */
/*   Updated: 2025/07/30 15:57:53 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
