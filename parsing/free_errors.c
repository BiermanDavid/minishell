/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_errors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:58:18 by dabierma          #+#    #+#             */
/*   Updated: 2025/08/20 05:29:18 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/**
 * Frees a file list and all its nodes.
 * Cleans up memory for redirections.
 */
static void	free_file_list(t_file_list *list)
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
static void	free_cmd_args(char **cmd)
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
 * Frees a command list and all its nodes
 * Cleans up all allocated memory
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

/**
 * Skips whitespace wrapper function,
 * part of token handling but doesnt fit anywhere else because norm func limit.
 */

void	skip_whitespace(const char *input, int *pos)
{
	while (input[*pos] == ' ' || input[*pos] == '\t')
		(*pos)++;
}
