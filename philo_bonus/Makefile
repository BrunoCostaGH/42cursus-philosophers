# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/30 14:13:47 by bsilva-c          #+#    #+#              #
#    Updated: 2023/04/06 18:06:20 by bsilva-c         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC=cc
CFLAGS=-Wall -Werror -Wextra -g 
DEPS=

SRCS=philo.c philo_utils.c proc_utils.c ft_atoi.c init_n_free.c
OBJS=$(SRCS:.c=.o)
NAME=philo

all: $(NAME)
$(NAME): $(OBJS) $(DEPS)
	@$(CC) $(OBJS) $(DEPS) -pthread -fsanitize=address -o $(NAME)
clean:
	@rm $(OBJS)
fclean: clean
	@rm $(NAME)
re: fclean all
