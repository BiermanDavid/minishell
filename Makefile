CC = gcc
CFLAGS = -Wall -Werror -Wextra -g -Iincludes
SRC_DIR = functions
OBJ_DIR = objects
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))
TARGET = shell

# Add readline library
LIBS = -lreadline

all: $(TARGET) message

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	@$(CC) $(CFLAGS) $(OBJECTS) $(LIBS) -o $(TARGET)

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

fclean: clean
	@rm -f $(TARGET)

re: fclean all

.PHONY: all clean fclean re message