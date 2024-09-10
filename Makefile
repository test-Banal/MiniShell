# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/28 11:54:39 by roarslan          #+#    #+#              #
#    Updated: 2024/09/03 16:59:04 by roarslan         ###   ########.fr        #
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
	parser3.c \
	expander1.c \
	expander2.c \
	pipex.c \
	pipex_utils.c \
	open_utils.c \
	get_next_line.c \
	get_next_line_utils.c

HEADER = minishell.h

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -lreadline $(CFLAGS) -g3 -o $(NAME)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

valgrind: all
	valgrind --suppressions=valgrind_readline_leaks_ignore.txt --leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=yes ./${NAME}

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re all valgrind

