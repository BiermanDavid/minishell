/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_type_unsigned.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 10:46:36 by dgessner          #+#    #+#             */
/*   Updated: 2025/03/29 22:15:25 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	type_unsigned(va_list args, int *len)
{
	unsigned int	nb;
	char			*str;
	unsigned int	i;

	nb = va_arg(args, unsigned int);
	str = ft_itoa_unsigned(nb);
	if (!str)
		return (-1);
	i = 0;
	while (str[i])
	{
		if (write(1, &str[i], 1) == -1)
		{
			free(str);
			return (-1);
		}
		i++;
	}
	*len += i;
	free(str);
	return (0);
}
