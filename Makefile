CC = gcc
CFLAGS = -Wall -Werror -Wextra -g -Iincludes
SRC_DIR = functions
OBJ_DIR = objects

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))
TARGET = minishell

# Add readline library
LIBS = -lreadline

all: $(TARGET) message

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJECTS) $(LIBFT) $(LIBS) -o $(TARGET)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

message:
	@echo "\033[32m▗▖  ▗▖▗▄▄▄▖▗▖  ▗▖▗▄▄▄▖ ▗▄▄▖▗▖ ▗▖▗▄▄▄▖▗▖   ▗▖       "
	@echo "▐▛▚▞▜▌  █  ▐▛▚▖▐▌  █  ▐▌   ▐▌ ▐▌▐▌   ▐▌   ▐▌       "
	@echo "▐▌  ▐▌  █  ▐▌ ▝▜▌  █   ▝▀▚▖▐▛▀▜▌▐▛▀▀▘▐▌   ▐▌       "
	@echo "▐▌  ▐▌▗▄█▄▖▐▌  ▐▌▗▄█▄▖▗▄▄▞▘▐▌ ▐▌▐▙▄▄▖▐▙▄▄▖▐▙▄▄▖     "  
	@echo " "                                  
	@echo "  42 Group Project by dabierma && dgessner"
	@echo "                    __   "
	@echo "         .,-;-;-,. /'_\\        Instructions[▼]:"
	@echo "       _/_/_/_|_\\_\\) /           [run ./shell]"
	@echo "     '-<_><_><_><_>=/\\"
	@echo "       \`/_/====/_/-'\\_\\"
	@echo "        \"\"     \"\"    \"\"\033[0m"

clean:
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@rm -f $(TARGET)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re message