/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 03:06:11 by dgessner          #+#    #+#             */
/*   Updated: 2025/03/16 07:20:46 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	a;
	unsigned int	s_len;

	a = 0;
	s_len = ft_strlen(s);
	while (a < s_len)
	{
		f(a, &s[a]);
		a++;
	}
}
