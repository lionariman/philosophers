#ifndef PHILO_TWO_H
# define PHILO_TWO_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <pthread.h>
# include <sys/time.h>
# include <semaphore.h>

unsigned int	died;

typedef struct	s_semaphores
{
	sem_t		**names;
	sem_t		*forks;
	sem_t		*s_write;
}				t_semaphores;

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
	pthread_t		philosopher;
	t_semaphores	*semaphore;
}				t_philosophers;

typedef struct	s_main
{
	char			**buff;
	long			time;
	t_arg			args;
	t_semaphores	*semaphore;
	t_philosophers *p;
}				t_main;

// typedef struct	s_semaphores
// {
// 	sem_t		**names;
// 	sem_t		*forks;
// 	sem_t		*s_write;
// }				t_semaphores;

// typedef struct	s_arg
// {
// 	long		num_of_p;
// 	long		time_to_die;
// 	long		time_to_eat;
// 	long		time_to_sleep;
// 	long		num_of_times_each_p_must_eat;
// }				t_arg;

// typedef struct	s_philosophers
// {
// 	int				id;
// 	long			start;
// 	long			last;
// 	t_arg			args;
// 	pthread_t		philosopher;
// 	t_semaphores	semaphore;
// }				t_philosophers;

// typedef struct	s_main
// {
// 	char			**buff;
// 	long			time;
// 	t_arg			args;
// 	t_semaphores	semaphore;
// 	t_philosophers *p;
// }				t_main;

// void	ft_putnbr_fd(long long int v, int fd);

#endif
