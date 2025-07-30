/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:07:09 by dgessner          #+#    #+#             */
/*   Updated: 2025/03/14 22:26:48 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strchr(char *string, int a)
{
	int	b;

	b = 0;
	while (string[b])
	{
		if (string[b] == (char)a)
			return (&string[b]);
		b++;
	}
	if ((char)a == '\0')
		return (&string[b]);
	return (0);
}
