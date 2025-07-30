/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 18:17:21 by dgessner          #+#    #+#             */
/*   Updated: 2025/03/17 11:47:30 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *pointer, int a, size_t len)
{
	size_t			c;
	unsigned char	*p;

	c = 0;
	p = (unsigned char *)pointer;
	while (c < len)
	{
		p[c] = (unsigned char)a;
		c++;
	}
	return (pointer);
}
