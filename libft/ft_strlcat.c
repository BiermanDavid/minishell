/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 12:47:34 by dgessner          #+#    #+#             */
/*   Updated: 2025/03/17 13:41:23 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	b;
	size_t	c;

	b = 0;
	while (b < dstsize && dst[b])
		b++;
	if (b == dstsize)
	{
		c = 0;
		while (src[c])
			c++;
		return (dstsize + c);
	}
	c = 0;
	while (src[c] && ((b + c) < (dstsize - 1)))
	{
		dst[b + c] = src[c];
		c++;
	}
	dst[b + c] = '\0';
	c = 0;
	while (src[c])
		c++;
	return (b + c);
}
