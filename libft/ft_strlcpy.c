/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 12:41:35 by dgessner          #+#    #+#             */
/*   Updated: 2025/03/17 13:41:07 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	b;
	size_t	c;

	b = 0;
	c = 0;
	if (dstsize != 0)
	{
		while (src[b] && (b < dstsize - 1))
		{
			dst[b] = src[b];
			b++;
		}
		dst[b] = '\0';
	}
	while (src[c])
		c++;
	return (c);
}
