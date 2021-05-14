#ifndef PHILO_ONE_H
# define PHILO_ONE_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <pthread.h>
# include <sys/time.h>

unsigned int	died;

typedef struct	s_mutex
{
	pthread_mutex_t	*forks;
	pthread_mutex_t m_died;
	pthread_mutex_t m_wait;
	pthread_mutex_t m_write;
}				t_mutex;

typedef struct	s_arg
{
	long		num_of_p;
	long		time_to_die;
	long		time_to_eat;
	long		time_to_sleep;
	long		num_of_times_each_p_must_eat;
}				t_arg;

typedef struct	s_philosophers
{
	int				id;
	long			start;
	long			last;
	t_arg			args;
	t_mutex			mutexes;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
	pthread_t		philosopher;
}				t_philosophers;

typedef struct	s_main
{
	long			time;

	t_arg			args;
	t_mutex			mutexes;
	t_philosophers *p;
}				t_main;

void	ft_putnbr_fd(long long int v, int fd);

#endif
