/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 18:35:33 by dabierma          #+#    #+#             */
/*   Updated: 2025/08/18 17:50:46 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/**
 * Processes single quoted strings (no expansion).
 * Everything inside single quotes is treated literally.
 */
char	*process_single_quotes(const char *input)
{
	int		len;
	char	*result;

	if (!input)
		return (NULL);
	len = ft_strlen(input);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	remove_quotes(result, input, len);
	return (result);
}

/**
 * Processes double quoted strings (with expansion).
 * Variables inside double quotes are expanded.
 */
char	*process_double_quotes(const char *input, char **envp)
{
	int		len;
	char	*temp;
	char	*result;

	if (!input)
		return (NULL);
	len = ft_strlen(input);
	temp = malloc(len + 1);
	if (!temp)
		return (NULL);
	remove_quotes(temp, input, len);
	result = expand_variables(temp, envp);
	free(temp);
	return (result);
}
