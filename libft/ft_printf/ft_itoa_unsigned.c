/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_unsigned.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 10:54:01 by dgessner          #+#    #+#             */
/*   Updated: 2025/03/28 10:58:51 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_num_len_unsigned(unsigned int n)
{
	unsigned int		len;
	unsigned long long	num;

	num = n;
	len = (n <= 0);
	while (num)
	{
		num /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa_unsigned(unsigned int n)
{
	char				*str;
	unsigned int		len;
	unsigned long long	num;

	len = ft_num_len_unsigned(n);
	num = n;
	if (n < 0)
		num = -num;
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	str[len] = '\0';
	while (len--)
	{
		str[len] = (num % 10) + '0';
		num /= 10;
	}
	if (n < 0)
		str[0] = '-';
	return (str);
}
