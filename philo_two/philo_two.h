#ifndef PHILO_TWO_H
# define PHILO_TWO_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <pthread.h>
# include <sys/time.h>
# include <semaphore.h>

typedef struct s_semaphores
{
	sem_t		*forks;
	sem_t		*s_write;
}				t_semaphores;

typedef struct s_arg
{
	long		num_of_p;
	long		time_to_die;
	long		time_to_eat;
	long		time_to_sleep;
	long		num_of_times_each_p_must_eat;
	int			died;
}				t_arg;

typedef struct s_philosophers
{
	int				id;
	long			start;
	long			last;
	t_arg			*args;
	t_semaphores	*semaphore;
	pthread_t		philosopher;
}				t_philosophers;

typedef struct s_main
{
	long			time;
	t_arg			*args;
	t_semaphores	*semaphore;
	t_philosophers	*p;
}				t_main;

// utils

long	m_atol(char *number);
int		m_isspace(char c);
int		m_isdigit(char c);
int		m_error(char *message);
int		m_strlen(char *str);

// time and free

void	m_free(t_main *m);
long	time_stamp(void);
void	m_sleep(long amount_of_time_to_wait);

// initialisation and validation

int		validation(char **av);
int		initialize(int ac, char **av, t_main *m);
int		init_semaphores(t_main *m);
int		init_philosophers(t_main *m);

// thread work

void	*death_checking(void *philosopher);
void	p_eating(t_philosophers *p);
void	p_sleeping(t_philosophers *p);
int		p_filling(t_philosophers *p, int *i);
void	p_thinking(t_philosophers *p);

// start thread

void	*threads(void *philosopher);
int		beginning(t_main *m);
int		main(int ac, char **av);

#endif
