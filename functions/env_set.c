/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 09:56:10 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/04 21:51:08 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

/**
 * Sets an environment value (new or replaces existing).
 */
static char	*create_env_entry(const char *key, const char *value)
{
	size_t	len;
	char	*new_entry;

	len = ft_strlen(key);
	new_entry = malloc(len + ft_strlen(value) + 2);
	if (!new_entry)
		return (NULL);
	sprintf(new_entry, "%s=%s", key, value);
	return (new_entry);
}

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

static char	**add_new_var(char ***envp, char *new_entry, int count)
{
	char	**new_env;

	if (!*envp)
		new_env = malloc(sizeof(char *) * 2);
	else
		new_env = realloc(*envp, sizeof(char *) * (count + 2));
	if (!new_env)
		return (NULL);
	new_env[count] = new_entry;
	new_env[count + 1] = NULL;
	*envp = new_env;
	return (new_env);
}

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
