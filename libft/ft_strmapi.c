/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 02:47:39 by dgessner          #+#    #+#             */
/*   Updated: 2025/03/17 13:39:56 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	a;
	unsigned int	s_len;
	char			*changedstr;

	if (!s || !f)
		return (NULL);
	a = 0;
	s_len = ft_strlen(s);
	changedstr = malloc(s_len + 1);
	if (!changedstr)
		return (NULL);
	while (a < s_len)
	{
		changedstr [a] = f(a, s[a]);
		a++;
	}
	changedstr[a] = '\0';
	return (changedstr);
}
