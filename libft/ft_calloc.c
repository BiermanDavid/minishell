/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 02:09:27 by dgessner          #+#    #+#             */
/*   Updated: 2025/03/17 11:51:35 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdint.h>

void	*ft_calloc(size_t count, size_t size)
{
	size_t	total;
	void	*array;

	if (count && size && (count > SIZE_MAX / size))
		return (NULL);
	total = count * size;
	array = malloc(total);
	if (!array)
		return (NULL);
	ft_memset(array, 0, total);
	return (array);
}
