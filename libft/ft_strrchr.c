/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:27:29 by dgessner          #+#    #+#             */
/*   Updated: 2025/03/17 13:39:30 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *string, int a)
{
	int	b;
	int	c;

	b = 0;
	c = -1;
	while (string[b])
	{
		if (string[b] == (char)a)
			c = b;
		b++;
	}
	if ((char)a == '\0')
		return ((char *)&string[b]);
	if (c == -1)
		return (NULL);
	return ((char *)&string[c]);
}
