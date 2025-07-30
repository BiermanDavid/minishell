/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:21:41 by dgessner          #+#    #+#             */
/*   Updated: 2025/03/17 11:55:11 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *string, int a, size_t b)
{
	const unsigned char	*str;
	size_t				c;

	c = 0;
	str = (const unsigned char *)string;
	while (c < b && str[c] != (unsigned char)a)
		c++;
	if (c == b)
		return (NULL);
	else
		return ((void *)&str[c]);
}
