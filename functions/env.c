/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 00:25:13 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/03 02:18:32 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

/**
 * Sucht nach einem Environment-Wert anhand des Keys.
 */
char	*env_get(char **env, const char *key)
{
	int		i;
	size_t	len;

	i = 0;
	len = strlen(key);
	while (env && env[i])
	{
		if (strncmp(env[i], key, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

/**
 * Setzt einen Environment-Wert (neu oder ersetzt bestehenden).
 */
int	env_set(char ***envp, const char *key, const char *value)
{
	int		i;
	size_t	len;
	char	*new_entry;
	char	**env;
	char	**new_env;

	i = 0;
	env = *envp;
	len = strlen(key);
	new_entry = malloc(len + strlen(value) + 2);
	if (!new_entry)
		return (1);
	sprintf(new_entry, "%s=%s", key, value);
	while (env && env[i])
	{
		if (strncmp(env[i], key, len) == 0 && env[i][len] == '=')
		{
			free(env[i]);
			env[i] = new_entry;
			return (0);
		}
		i++;
	}
	if (!env)
		new_env = malloc(sizeof(char *) * 2);
	else
		new_env = realloc(env, sizeof(char *) * (i + 2));

	if (!new_env)
	{
		free(new_entry);
		return (1);
	}
	new_env[i] = new_entry;
	new_env[i + 1] = NULL;
	*envp = new_env;
	return (0);
}

/**
 * Entfernt einen Environment-Wert anhand des Keys.
 */
int	env_unset(char ***envp, const char *key)
{
	char	**env;
	int		i;
	int		j;
	size_t	len;

	i = 0;
	env = *envp;
	len = strlen(key);
	while (env && env[i])
	{
		if (strncmp(env[i], key, len) == 0 && env[i][len] == '=')
		{
			free(env[i]);
			j = i;
			while (env[j])
			{
				env[j] = env[j + 1];
				j++;
			}
			return (0);
		}
		i++;
	}
	return (1);
}

/**
 * Gibt alle aktuellen Environment-Variablen aus.
 */
void	env_print(char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
}

char **duplicate_env(char **envp)
{
	int		i;
	char	**new_env;

	for (i = 0; envp && envp[i]; i++);
	new_env = malloc(sizeof(char *) * (i + 1));
	if (!new_env)
		return (NULL);
	for (i = 0; envp && envp[i]; i++)
		new_env[i] = strdup(envp[i]);
	new_env[i] = NULL;
	return (new_env);
}

void free_env(char **env)
{
	int i = 0;
	while (env && env[i])
		free(env[i++]);
	free(env);
}