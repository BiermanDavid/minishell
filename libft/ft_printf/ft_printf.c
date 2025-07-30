/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 20:38:22 by dgessner          #+#    #+#             */
/*   Updated: 2025/03/29 21:44:16 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_clean_exit(va_list *args)
{
	va_end(*args);
	return (-1);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		i;
	int		len;

	i = 0;
	len = 0;
	va_start(args, format);
	while (format[i])
	{
		if (format[i] == '%' && format[i + 1])
		{
			i++;
			if (ft_search_type(format, &i, args, &len) == -1)
				return (ft_clean_exit(&args));
		}
		else
		{
			if (write(1, &format[i], 1) == -1)
				return (ft_clean_exit(&args));
			len++;
		}
		i++;
	}
	va_end(args);
	return (len);
}
