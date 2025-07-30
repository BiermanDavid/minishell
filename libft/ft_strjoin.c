/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 21:51:10 by dgessner          #+#    #+#             */
/*   Updated: 2025/03/17 13:41:27 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char			*joinedstrings;
	unsigned int	len_s1;
	unsigned int	len_s2;
	unsigned int	i;

	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	i = 0;
	joinedstrings = malloc(len_s1 + len_s2 + 1);
	if (!joinedstrings)
		return ((void *)0);
	while (i < len_s1)
	{
		joinedstrings[i] = s1[i];
		i++;
	}
	i = 0;
	while (i < len_s2)
	{
		joinedstrings[len_s1 + i] = s2[i];
		i++;
	}
	joinedstrings[len_s1 + len_s2] = '\0';
	return (joinedstrings);
}
