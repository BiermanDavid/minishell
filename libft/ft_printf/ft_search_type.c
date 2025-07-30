/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_search_type.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 20:58:42 by dgessner          #+#    #+#             */
/*   Updated: 2025/03/29 21:54:13 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_search_type(const char *format, int *i, va_list args, int *len)
{
	if (format[*i] == 's')
		return (type_string(args, len));
	else if (format[*i] == 'd' || format[*i] == 'i')
		return (type_decimal(args, len));
	else if (format[*i] == 'c')
		return (type_char(args, len));
	else if (format[*i] == 'p')
		return (type_pointer(args, len));
	else if (format[*i] == 'u')
		return (type_unsigned(args, len));
	else if (format[*i] == 'x')
		return (type_hex(args, len, 0));
	else if (format[*i] == 'X')
		return (type_hex(args, len, 1));
	else if (format[*i] == '%')
	{
		if (write(1, "%", 1) == -1)
			return (-1);
		(*len)++;
		return (0);
	}
	return (-1);
}
