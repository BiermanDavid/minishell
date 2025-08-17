/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_expansion.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 20:46:00 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/17 21:16:06 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "parse.h"

/**
 * Rebuilds command array by replacing one argument with multiple arguments.
 * Used when wildcard expansion produces multiple matches.
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
 * Processes wildcard expansion for a single argument.
 * Returns new command array if expansion occurred, NULL otherwise.
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
 * Processes a single argument expansion with variable substitution.
 * Returns 1 if wildcards expanded (exit early), 0 to continue.
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
 * Processes a single argument expansion without variable substitution.
 * Returns 1 if wildcards expanded (exit early), 0 to continue.
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
 * Expands variables and wildcards in command arguments during execution.
 * Modifies the command node's argument array in place.
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
