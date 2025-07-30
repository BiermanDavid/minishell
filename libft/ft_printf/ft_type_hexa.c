/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_type_hexa.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 11:11:30 by dgessner          #+#    #+#             */
/*   Updated: 2025/03/29 22:07:42 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	type_hex(va_list args, int *len, int upper)
{
	unsigned int	n;

	n = va_arg(args, unsigned int);
	if (upper)
	{
		if (ft_puthex_upper(n, len) == -1)
			return (-1);
	}
	else
	{
		if (ft_puthex(n, len) == -1)
			return (-1);
	}
	return (0);
}
