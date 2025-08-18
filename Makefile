CC = gcc
CFLAGS = -Wall -Werror -Wextra -g -Iincludes
PARSING_DIR = parsing
EXECUTION_DIR = execution
OBJ_DIR = objects

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
PARSING_SOURCES = $(wildcard $(PARSING_DIR)/*.c)
EXECUTION_SOURCES = $(wildcard $(EXECUTION_DIR)/*.c)
SOURCES = $(PARSING_SOURCES) $(EXECUTION_SOURCES)
PARSING_OBJECTS = $(patsubst $(PARSING_DIR)/%.c, $(OBJ_DIR)/%.o, $(PARSING_SOURCES))
EXECUTION_OBJECTS = $(patsubst $(EXECUTION_DIR)/%.c, $(OBJ_DIR)/%.o, $(EXECUTION_SOURCES))
OBJECTS = $(PARSING_OBJECTS) $(EXECUTION_OBJECTS)
TARGET = minishell

# Add readline library
LIBS = -lreadline

all: $(TARGET) message

$(OBJ_DIR)/%.o: $(PARSING_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(EXECUTION_DIR)/%.c
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