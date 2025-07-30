/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 14:53:14 by dgessner          #+#    #+#             */
/*   Updated: 2025/03/17 13:35:38 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t a)
{
	size_t			b;
	unsigned char	*source;
	unsigned char	*destination;

	b = 0;
	source = (unsigned char *)src;
	destination = (unsigned char *)dest;
	if (!dest && !src)
		return (NULL);
	while (b < a)
	{
		destination[b] = source[b];
		b++;
	}
	return (dest);
}
