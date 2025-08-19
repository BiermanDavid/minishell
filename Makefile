CC = gcc
CFLAGS = -Wall -Werror -Wextra -g -Iincludes
LIBS = -lreadline

PARSING_DIR = parsing
EXECUTION_DIR = execution

OBJ_DIR = objects

LIBFT = $(LIBFT_DIR)/libft.a

PARSING_SOURCES = $(PARSING_DIR)/expansion_quotes_mixed.c \
$(PARSING_DIR)/expansion_quotes_utils.c \
$(PARSING_DIR)/expansion_quotes.c \
$(PARSING_DIR)/expansion.c \
$(PARSING_DIR)/expansion_utils.c \
$(PARSING_DIR)/free_errors.c \
$(PARSING_DIR)/handle_inputs_reader.c \
$(PARSING_DIR)/handle_inputs.c \
$(PARSING_DIR)/heredoc_utils.c \
$(PARSING_DIR)/heredoc.c \
$(PARSING_DIR)/lexer_utils.c \
$(PARSING_DIR)/lexer_word.c \
$(PARSING_DIR)/lexer.c \
$(PARSING_DIR)/main.c \
$(PARSING_DIR)/make_parser_nodes.c \
$(PARSING_DIR)/memory_utils.c \
$(PARSING_DIR)/parse_syntax.c \
$(PARSING_DIR)/parse.c \
$(PARSING_DIR)/parser_cmd.c \
$(PARSING_DIR)/redirect_utils.c \
$(PARSING_DIR)/redirects.c \
$(PARSING_DIR)/signal_utils.c \
$(PARSING_DIR)/signal.c \
$(PARSING_DIR)/wildcard_expansion.c \
$(PARSING_DIR)/wildcard_utils.c \

EXECUTION_SOURCES = $(EXECUTION_DIR)/builtin_cd.c \
$(EXECUTION_DIR)/builtins_env.c \
$(EXECUTION_DIR)/builtins.c \
$(EXECUTION_DIR)/cmd_array_utils.c \
$(EXECUTION_DIR)/command_validation.c \
$(EXECUTION_DIR)/env_helper.c \
$(EXECUTION_DIR)/env_set.c \
$(EXECUTION_DIR)/env.c \
$(EXECUTION_DIR)/error.c \
$(EXECUTION_DIR)/execute_builtins.c \
$(EXECUTION_DIR)/execute_expansion.c \
$(EXECUTION_DIR)/execute_process.c \
$(EXECUTION_DIR)/execute_redirect.c \
$(EXECUTION_DIR)/execute_utils.c \
$(EXECUTION_DIR)/execute.c \
$(EXECUTION_DIR)/pipex_exec.c \
$(EXECUTION_DIR)/pipex_utils.c \
$(EXECUTION_DIR)/pipex.c \

SOURCES = $(PARSING_SOURCES) $(EXECUTION_SOURCES)

PARSING_OBJECTS = $(OBJ_DIR)/expansion_quotes_mixed.o \
$(OBJ_DIR)/expansion_quotes_utils.o \
$(OBJ_DIR)/expansion_quotes.o \
$(OBJ_DIR)/expansion_utils.o \
$(OBJ_DIR)/expansion.o \
$(OBJ_DIR)/free_errors.o \
$(OBJ_DIR)/handle_inputs_reader.o \
$(OBJ_DIR)/handle_inputs.o \
$(OBJ_DIR)/heredoc_utils.o \
$(OBJ_DIR)/heredoc.o \
$(OBJ_DIR)/lexer_utils.o \
$(OBJ_DIR)/lexer_word.o \
$(OBJ_DIR)/lexer.o \
$(OBJ_DIR)/main.o \
$(OBJ_DIR)/make_parser_nodes.o \
$(OBJ_DIR)/memory_utils.o \
$(OBJ_DIR)/parse_syntax.o \
$(OBJ_DIR)/parse.o \
$(OBJ_DIR)/parser_cmd.o \
$(OBJ_DIR)/redirect_utils.o \
$(OBJ_DIR)/redirects.o \
$(OBJ_DIR)/signal_utils.o \
$(OBJ_DIR)/signal.o \
$(OBJ_DIR)/wildcard_expansion.o \
$(OBJ_DIR)/wildcard_utils.o
EXECUTION_OBJECTS = $(OBJ_DIR)/builtin_cd.o \
$(OBJ_DIR)/builtins_env.o \
$(OBJ_DIR)/builtins.o \
$(OBJ_DIR)/cmd_array_utils.o \
$(OBJ_DIR)/env_helper.o \
$(OBJ_DIR)/env_set.o \
$(OBJ_DIR)/env.o \
$(OBJ_DIR)/error.o \
$(OBJ_DIR)/execute_builtins.o \
$(OBJ_DIR)/execute_expansion.o \
$(OBJ_DIR)/execute_process.o \
$(OBJ_DIR)/execute_redirect.o \
$(OBJ_DIR)/execute_utils.o \
$(OBJ_DIR)/execute.o \
$(OBJ_DIR)/pipex_exec.o \
$(OBJ_DIR)/pipex_utils.o \
$(OBJ_DIR)/pipex.o

OBJECTS = $(PARSING_OBJECTS) $(EXECUTION_OBJECTS)

TARGET = minishell
LIBFT_DIR = libft

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