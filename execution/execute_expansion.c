/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_expansion.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 20:46:00 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/20 06:25:53 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "parse.h"

/**
 * Reconstructs command argument array with expanded wildcard replacements.
 * Dynamically reallocates memory to accommodate multiple file matches from
 * single wildcard pattern, preserving argument order and command structure.
 * Essential for proper shell glob expansion during command execution.
 * @param old_cmd Original command array to modify
 * @param index Position of argument to replace with expansions
 * @param replacements Array of expanded filenames from wildcard matching
 * @param replacement_count Number of replacement strings to insert
 * @return Newly allocated command array with expansions, NULL on failure
 */
static char	**rebuild_cmd_array(char **old_cmd, int index,
		char **replacements, int replacement_count)
{
	char	**new_cmd;
	int		old_count;
	int		new_count;
	int		i;
	int		new_i;

	old_count = count_args(old_cmd);
	new_count = old_count - 1 + replacement_count;
	new_cmd = malloc(sizeof(char *) * (new_count + 1));
	if (!new_cmd)
		return (NULL);
	new_i = 0;
	i = 0;
	while (i < old_count)
	{
		if (i == index)
			copy_replacements(new_cmd, &new_i, replacements, replacement_count);
		else
			copy_single_arg(new_cmd, &new_i, old_cmd[i]);
		i++;
	}
	new_cmd[new_count] = NULL;
	return (new_cmd);
}

/**
 * Attempts wildcard expansion on specified command argument.
 * Searches filesystem for pattern matches and rebuilds command array
 * if expansions found. Handles memory management for both successful
 * and failed expansion attempts to prevent leaks during glob processing.
 * @param node Command node containing argument array to expand
 * @param i Index of argument to attempt wildcard expansion on
 * @param arg_to_expand String containing potential wildcard patterns
 * @return New command array if expansion succeeded, NULL if no matches
 */
static char	**process_wildcard_expansion(t_cmd_node *node, int i,
		char *arg_to_expand)
{
	char	**wildcard_matches;
	char	**new_cmd;
	int		match_count;

	wildcard_matches = expand_wildcards(arg_to_expand, &match_count);
	if (wildcard_matches)
	{
		new_cmd = rebuild_cmd_array(node->cmd, i, wildcard_matches,
				match_count);
		if (new_cmd)
		{
			free_split(node->cmd);
			free_split(wildcard_matches);
			return (new_cmd);
		}
		free_split(wildcard_matches);
	}
	return (NULL);
}

/**
 * Handles argument expansion after environment variable substitution.
 * Attempts wildcard expansion on already-substituted argument string,
 * then updates command node with either expanded array or single substituted 
 * value. Returns early termination flag to prevent 
 * further processing on successful expansion.
 * @param node Command node to modify with expanded arguments
 * @param i Index of argument being processed in command array
 * @param expanded Pre-substituted argument string to potentially expand
 * @return 1 if wildcard expansion occurred (stop processing), 0 to continue
 */
static int	process_expanded_argument(t_cmd_node *node, int i, char *expanded)
{
	char	**new_cmd;

	new_cmd = process_wildcard_expansion(node, i, expanded);
	if (new_cmd)
	{
		node->cmd = new_cmd;
		free(expanded);
		return (1);
	}
	free(node->cmd[i]);
	node->cmd[i] = expanded;
	return (0);
}

/**
 * Attempts wildcard expansion on unmodified command argument.
 * Directly processes original argument string for glob patterns without
 * prior variable substitution, updating command array if matches found.
 * Used when no environment variables detected in argument string.
 * @param node Command node containing argument array to potentially expand
 * @param i Index of argument to check for wildcard expansion
 * @return 1 if wildcard expansion occurred (stop processing), 0 to continue
 */
static int	process_direct_argument(t_cmd_node *node, int i)
{
	char	**new_cmd;

	new_cmd = process_wildcard_expansion(node, i, node->cmd[i]);
	if (new_cmd)
	{
		node->cmd = new_cmd;
		return (1);
	}
	return (0);
}

/**
 * Performs complete argument expansion pipeline for command execution.
 * Sequentially processes each command argument for environment variable
 * substitution followed by wildcard expansion, modifying command node
 * structure in-place. Terminates early if wildcard expansion reconstructs
 * argument array to prevent index invalidation during iteration.
 * @param node Command node with argument array requiring expansion
 * @param envp Environment variables array for substitution context
 */
void	expand_command_args(t_cmd_node *node, char **envp)
{
	int		i;
	char	*expanded;

	i = 0;
	while (node->cmd[i])
	{
		expanded = expand_token(node->cmd[i], envp);
		if (expanded)
		{
			if (process_expanded_argument(node, i, expanded))
				return ;
		}
		else
		{
			if (process_direct_argument(node, i))
				return ;
		}
		i++;
	}
}
