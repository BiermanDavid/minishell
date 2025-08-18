/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 09:56:10 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/18 14:51:32 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

/**
 * Creates a new environment entry string in format "key=value".
 * Returns allocated string or NULL on failure.
 */
static char	*create_env_entry(const char *key, const char *value)
{
	size_t	len;
	char	*new_entry;

	len = ft_strlen(key);
	new_entry = malloc(len + ft_strlen(value) + 2);
	if (!new_entry)
		return (NULL);
	ft_strlcpy(new_entry, key, len + ft_strlen(value) + 2);
	ft_strlcat(new_entry, "=", len + ft_strlen(value) + 2);
	ft_strlcat(new_entry, value, len + ft_strlen(value) + 2);
	return (new_entry);
}

/**
 * Replaces existing environment variable with new entry.
 * Returns 1 if variable was found and replaced, 0 otherwise.
 */
static int	replace_existing_var(char **env, const char *key, char *new_entry)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(key);
	while (env && env[i])
	{
		if (ft_strncmp(env[i], key, len) == 0 && env[i][len] == '=')
		{
			free(env[i]);
			env[i] = new_entry;
			return (1);
		}
		i++;
	}
	return (0);
}

/**
 * Adds a new variable to the environment array.
 * Expands the array and places new entry at the end.
 */
static char	**add_new_var(char ***envp, char *new_entry, int count)
{
	char	**new_env;
	int		i;

	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < count && *envp)
	{
		new_env[i] = (*envp)[i];
		i++;
	}
	new_env[count] = new_entry;
	new_env[count + 1] = NULL;
	if (*envp)
		free(*envp);
	*envp = new_env;
	return (new_env);
}

/**
 * Sets an environment variable to the specified value.
 * Creates new entry or replaces existing one.
 * Returns 0 on success, 1 on failure.
 */
int	env_set(char ***envp, const char *key, const char *value)
{
	char	*new_entry;
	int		count;

	new_entry = create_env_entry(key, value);
	if (!new_entry)
		return (1);
	if (replace_existing_var(*envp, key, new_entry))
		return (0);
	count = 0;
	while (*envp && (*envp)[count])
		count++;
	if (!add_new_var(envp, new_entry, count))
	{
		free(new_entry);
		return (1);
	}
	return (0);
}
