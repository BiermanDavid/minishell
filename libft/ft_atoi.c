/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 18:24:41 by dgessner          #+#    #+#             */
/*   Updated: 2025/03/17 11:31:11 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	int	result;
	int	isneg;

	result = 0;
	isneg = 0;
	while ((*str >= 9 && *str <= 13) || (*str == 32))
		str++;
	if (*str == ('-') || *str == '+')
	{
		if (*str == '-')
			isneg = 1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = (result * 10 + (*str - '0'));
		str++;
	}
	if (isneg == 1)
		result = result * -1;
	return (result);
}
