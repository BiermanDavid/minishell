/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_type_decimal.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 20:39:25 by dgessner          #+#    #+#             */
/*   Updated: 2025/03/29 22:14:56 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	type_decimal(va_list args, int *len)
{
	int		nb;
	char	*str;
	int		i;

	nb = va_arg(args, int);
	str = ft_itoa(nb);
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
