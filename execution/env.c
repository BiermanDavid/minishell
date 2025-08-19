/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 00:25:13 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/19 21:16:41 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

/**
 * Searches for an environment value by key.
 */
char	*env_get(char **env, const char *key)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(key);
	while (env && env[i])
	{
		if (ft_strncmp(env[i], key, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

/**
 * Removes an environment value by key.
 */
int	env_unset(char ***envp, const char *key)
{
	char	**env;
	int		i;
	int		j;
	size_t	len;

	i = 0;
	env = *envp;
	len = ft_strlen(key);
	while (env && env[i])
	{
		if (ft_strncmp(env[i], key, len) == 0 && env[i][len] == '=')
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
