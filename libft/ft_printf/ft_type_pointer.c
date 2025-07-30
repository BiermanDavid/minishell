/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_type_pointer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 09:52:56 by dgessner          #+#    #+#             */
/*   Updated: 2025/03/29 22:09:16 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	type_pointer(va_list args, int *len)
{
	unsigned long long	a;

	a = (unsigned long long)va_arg(args, void *);
	if (write(1, "0x", 2) == -1)
		return (-1);
	(*len) += 2;
	if (ft_puthex(a, len) == -1)
		return (-1);
	return (0);
}
