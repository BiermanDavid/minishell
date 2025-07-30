/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:29:39 by dgessner          #+#    #+#             */
/*   Updated: 2025/03/17 11:56:04 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t				c;
	const unsigned char	*str1;
	const unsigned char	*str2;

	c = 0;
	str1 = (const unsigned char *)s1;
	str2 = (const unsigned char *)s2;
	while (c < n)
	{
		if (str1[c] != str2[c])
			return (str1[c] - str2[c]);
		c++;
	}
	return (0);
}
