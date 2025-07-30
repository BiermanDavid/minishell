/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_type_char.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 15:15:36 by dgessner          #+#    #+#             */
/*   Updated: 2025/03/29 22:00:32 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	type_char(va_list args, int *len)
{
	char	a;

	a = (char)va_arg(args, int);
	if (write(1, &a, 1) == -1)
		return (-1);
	(*len) += 1;
	return (0);
}
