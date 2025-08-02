/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 00:25:13 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/03 00:25:37 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

// Hilfsfunktionen
char *env_get(char **env, const char *key)
{
	int i = 0;
	size_t len = strlen(key);

	while (env && env[i])
	{
		if (strncmp(env[i], key, len) == 0 && env[i][len] == '=')
			return env[i] + len + 1;
		i++;
	}
	return NULL;
}

int env_set(char ***envp, const char *key, const char *value)
{
	int i = 0;
	size_t len = strlen(key);
	char *new_entry;
	char **env = *envp;

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

	// Key not found – realloc env array
	char **new_env = realloc(env, sizeof(char *) * (i + 2));
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

int env_unset(char ***envp, const char *key)
{
	char **env = *envp;
	int i = 0, j = 0;
	size_t len = strlen(key);

	while (env && env[i])
	{
		if (strncmp(env[i], key, len) == 0 && env[i][len] == '=')
		{
			free(env[i]);
			for (j = i; env[j]; j++)
				env[j] = env[j + 1];
			return (0);
		}
		i++;
	}
	return (1);
}

void env_print(char **env)
{
	int i = 0;
	while (env && env[i])
		printf("%s\n", env[i++]);
}