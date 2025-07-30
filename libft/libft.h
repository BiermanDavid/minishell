/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 03:11:37 by dgessner          #+#    #+#             */
/*   Updated: 2025/03/17 13:47:29 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

int		ft_atoi(const char *str);
void	ft_bzero(void *str, size_t n);
void	*ft_calloc(size_t count, size_t size);
int		ft_isalnum(int a);
int		ft_isalpha(int a);
int		ft_isascii(int a);
int		ft_isdigit(int a);
int		ft_isprint(int a);
char	*ft_itoa(int n);
void	*ft_memchr(const void *string, int a, size_t b);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t a);
void	*ft_memmove(void *dest, const void *src, size_t a);
void	*ft_memset(void *pointer, int a, size_t len);
void	ft_putchar_fd(char c, int fd);
void	ft_putendl_fd(char *c, int fd);
void	ft_putnbr_fd(int n, int fd);
void	ft_putstr_fd(char *c, int fd);
char	**ft_split(char const *s, char c);
char	*ft_strchr(char *string, int a);
char	*ft_strdup(const char *s1);
void	ft_striteri(char *s, void (*f)(unsigned int, char*));
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlcat(char *dst, const char *src, size_t dstsize);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t	ft_strlen(const char	*string);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
int		ft_strncmp(const char *string1, const char *string2,
			size_t a);
char	*ft_strnstr(const char *haystack, const char *needle,
			size_t len);
char	*ft_strrchr(const char *string, int a);
char	*ft_strtrim(char const *s1, char const *set);
char	*ft_substr(char const *s, size_t start, size_t len);
int		ft_tolower(int a);
int		ft_toupper(int a);

#endif
