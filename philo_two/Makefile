# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: keuclide <keuclide@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/05/20 04:34:40 by keuclide          #+#    #+#              #
#    Updated: 2021/05/20 04:37:31 by keuclide         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo_two

CC = gcc
FL = -Wall -Wextra -Werror

SRCS =	philo_two.c \
		initialisation_validation.c \
		thread_work.c \
		time_free.c \
		utils.c

OBJS = $(SRCS:.c=.o)

.c.o:
	$(CC) $(FL) -o $@ -c $^

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean:
	rm -f $(OBJS)
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re