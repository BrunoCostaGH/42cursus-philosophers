# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/16 12:38:11 by bsilva-c          #+#    #+#              #
#    Updated: 2023/03/27 16:13:31 by bsilva-c         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC=cc
CFLAGS=-Wall -Werror -Wextra -g 
DEPS=

SRCS=philo.c philo_utils.c philo_eat.c fork_utils.c ft_atoi.c init_n_free.c
OBJS=$(SRCS:.c=.o)
NAME=philo

all: $(NAME)
$(NAME): $(OBJS) $(DEPS)
	@$(CC) $(OBJS) $(DEPS) -pthread -o $(NAME)
clean:
	@rm $(OBJS)
fclean: clean
	@rm $(NAME)
re: fclean all
