/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_three.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keuclide <keuclide@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/14 15:09:29 by keuclide          #+#    #+#             */
/*   Updated: 2021/05/19 05:15:20 by keuclide         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_THREE_H
# define PHILO_THREE_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <pthread.h>
# include <sys/time.h>
# include <semaphore.h>
# include <signal.h>

unsigned int	died;

typedef struct	s_semaphores
{
	sem_t	**names;
	sem_t	*forks;
	sem_t	*s_write;
	sem_t	*s_full;
	sem_t	*s_died;
	sem_t	*s_parent_status;
	sem_t	*s_child_status;
}				t_semaphores;

typedef struct	s_arg
{
	long	num_of_p;
	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;
	long	num_of_times_each_p_must_eat;
}				t_arg;

typedef struct	s_philosophers
{
	int				id;
	long			start;
	long			last;
	pid_t			*pid;
	t_arg			args;
	pthread_t		philosopher;
	t_semaphores	*semaphore;
}				t_philosophers;

typedef struct	s_main
{
	char			**buff;
	long			time;
	pid_t			*pid;
	t_arg			args;
	t_semaphores	*semaphore;
	t_philosophers *p;
}				t_main;

#endif