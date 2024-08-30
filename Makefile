# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/28 11:54:39 by roarslan          #+#    #+#              #
#    Updated: 2024/08/27 13:18:15 by roarslan         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = minishell

CC = cc

CFLAGS = -Wall -Wextra -Werror

SRC = main.c \
	utils1.c \
	utils2.c \
	utils3.c \
	parsing0.c \
	parsing1.c \
	parsing2.c \
	ft_split.c \
	env.c \
	is_cmd.c \
	quotes_double.c \
	built_in.c \
	lexer.c \
	parser1.c \
	parser2.c \
	parser3.c \
	expander1.c \
	expander2.c

HEADER = minishell.h

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -lreadline $(CFLAGS) -o $(NAME)

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

