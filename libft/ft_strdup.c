/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 02:44:26 by dgessner          #+#    #+#             */
/*   Updated: 2025/03/15 19:59:00 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char	*ft_strdup(const char *s1)
{
	unsigned int	total;
	unsigned int	a;
	char			*array;	

	total = 0;
	a = 0;
	while (s1[total])
		total++;
	array = malloc((total + 1) * sizeof(char));
	if (!array)
		return ((void *)0);
	while (total > a)
	{
		array[a] = s1[a];
		a++;
	}
	array[a] = '\0';
	return (array);
}
