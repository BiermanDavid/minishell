/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 20:25:32 by dgessner          #+#    #+#             */
/*   Updated: 2025/03/29 22:11:33 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>

int		ft_printf(const char *format, ...);
int		ft_search_type(const char *format, int *i, va_list args, int *len);
int		type_decimal(va_list args, int *len);
int		type_string(va_list args, int *len);
int		type_char(va_list args, int *len);
int		type_pointer(va_list args, int *len);
int		type_unsigned(va_list args, int *len);
int		type_hex(va_list args, int *len, int upper);
char	*ft_itoa(int n);
char	*ft_itoa_unsigned(unsigned int n);
int		ft_puthex(unsigned long long n, int *len);
int		ft_puthex_upper(unsigned int n, int *len);

#endif
