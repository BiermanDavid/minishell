# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/21 20:36:01 by dgessner          #+#    #+#              #
#    Updated: 2025/03/29 22:14:15 by dgessner         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = libftprintf.a

CC          = cc
CFLAGS      = -Wall -Wextra -Werror

SRC         = ft_printf.c \
              ft_search_type.c \
              ft_type_decimal.c \
              ft_type_string.c \
			  ft_type_char.c \
			  ft_type_pointer.c \
			  ft_type_hexa.c \
			  ft_type_unsigned.c \
			  ft_itoa_unsigned.c \
			  ft_itoa.c \
			  ft_puthexa.c \

OBJ         = $(SRC:.c=.o)

# Default-Ziel
all: $(NAME)

# printf-Library bauen
$(NAME): $(OBJ)
	ar rc $(NAME) $(OBJ)

# Objektdateien bauen
%.o: %.c
	$(CC) $(CFLAGS) -I. -c $< -o $@

# Test-Programm bauen
test: all
	$(CC) $(CFLAGS) -I. main.c $(NAME) -o test.out

# Aufräumen
clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME) test.out

re: fclean all

.PHONY: all clean fclean re test
