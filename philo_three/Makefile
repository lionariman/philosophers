# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: keuclide <keuclide@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/05/20 04:52:16 by keuclide          #+#    #+#              #
#    Updated: 2021/05/20 04:55:50 by keuclide         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo_three

CC = gcc
FL = -Wall -Wextra -Werror

SRCS =	philo_three.c \
		initialisation_validation.c \
		process_work.c \
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