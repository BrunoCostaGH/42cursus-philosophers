# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/16 12:38:11 by bsilva-c          #+#    #+#              #
#    Updated: 2023/03/16 12:45:23 by bsilva-c         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC=cc
CFLAGS=-Wall -Werror -Wextra -pthread
DEPS=

SRCS=main.c
OBJS=$(SRCS:.c=.o)
NAME=philo

all: $(NAME)
$(NAME): $(OBJS) $(DEPS)
	$(CC) $(OBJS) $(DEPS) -o $(NAME)
clean:
	@rm $(OBJS)
fclean: clean
	@rm $(NAME)
re: fclean all
