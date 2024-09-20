# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aneumann <aneumann@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/28 11:54:39 by roarslan          #+#    #+#              #
#    Updated: 2024/09/20 16:27:22 by aneumann         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc

CFLAGS = -Wall -Wextra -Werror

SRC = main.c \
	utils1.c \
	utils2.c \
	utils3.c \
	utils4.c \
	utils5.c \
	parsing0.c \
	parsing1.c \
	parsing2.c \
	ft_split.c \
	env1.c \
	env2.c \
	is_cmd.c \
	quotes_double.c \
	built_in.c \
	unset.c \
	export.c \
	ft_exit.c \
	ft_echo.c \
	ft_cd.c \
	lexer.c \
	parser1.c \
	parser2.c \
	expander1.c \
	expander2.c \
	expander3.c \
	pipex.c \
	piping.c \
	pipex_utils.c \
	open_utils.c \
	get_next_line.c \
	get_next_line_utils.c \
	signal.c \
	syntax_check.c \
	heredoc.c \
	ft_error.c

HEADER = minishell.h

OBJ_DIR = obj

OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -lreadline $(CFLAGS) -g3 -o $(NAME)

$(OBJ_DIR)/%.o: %.c $(HEADER)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

valgrind: $(NAME)
	valgrind --suppressions=valgrind_readline_leaks_ignore.txt --leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=yes ./${NAME}

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re all valgrind
