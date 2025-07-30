/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puthexa.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 11:19:09 by dgessner          #+#    #+#             */
/*   Updated: 2025/03/29 22:08:32 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_puthex(unsigned long long n, int *len)
{
	char	*base;

	base = "0123456789abcdef";
	if (n >= 16)
	{
		if (ft_puthex(n / 16, len) == -1)
			return (-1);
	}
	if (write(1, &base[n % 16], 1) == -1)
		return (-1);
	(*len)++;
	return (0);
}

int	ft_puthex_upper(unsigned int n, int *len)
{
	char	*base;

	base = "0123456789ABCDEF";
	if (n >= 16)
	{
		if (ft_puthex_upper(n / 16, len) == -1)
			return (-1);
	}
	if (write(1, &base[n % 16], 1) == -1)
		return (-1);
	(*len)++;
	return (0);
}
