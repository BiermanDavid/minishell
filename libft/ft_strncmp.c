/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:31:51 by dgessner          #+#    #+#             */
/*   Updated: 2025/03/17 13:39:46 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *string1, const char *string2, size_t a)
{
	size_t	b;

	b = 0;
	while ((string1[b] != '\0' || string2[b] != '\0') && b < a)
	{
		if ((unsigned char)string1[b] != (unsigned char)string2[b])
			return ((unsigned char)string1[b] - (unsigned char)string2[b]);
		b++;
	}
	return (0);
}
