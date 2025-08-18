/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 03:20:40 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/18 17:56:30 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

/**
 * Creates a deep copy of the environment array.
 * Returns a new environment array with duplicated strings.
 */
char	**duplicate_env(char **envp)
{
	int		i;
	char	**new_env;

	i = 0;
	while (envp && envp[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (envp && envp[i])
	{
		new_env[i] = ft_strdup(envp[i]);
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

/**
 * Frees all memory allocated for an environment array.
 * Frees each string and the array itself.
 */
void	free_env(char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

/**
 * Checks if a string is a valid environment variable identifier.
 * Must start with letter or underscore, contain only alphanumeric 
 * chars and underscores.
 */
int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

/**
 * Checks if a string is a valid variable assignment (var=value).
 * Validates the identifier part before the equals sign.
 */
int	is_assignment(const char *str)
{
	int		i;
	char	*eq;

	i = 0;
	eq = ft_strchr((char *)str, '=');
	if (!eq || eq == str)
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	while (str + i < eq)
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
