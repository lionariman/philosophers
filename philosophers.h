#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <pthread.h>
# include <sys/time.h>

typedef enum {
	FULL,
	ALIVE,
	DEAD
}	t_status;

typedef struct	s_mutex
{
	// we need as many forks as philosophers
	pthread_mutex_t	*forks;

	//..............
	pthread_mutex_t died;

	//..............
	pthread_mutex_t mstat;
}				t_mutex;

// p - philospher

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
	//philosopher id
	int				id;

	//................
	long			start;

	//................
	long			last;

	t_arg			args;

	t_mutex			mutexes;


	pthread_mutex_t	l_fork;
	pthread_mutex_t	r_fork;


	//................
	pthread_t		thread;


	//................
	t_status		fad;
}				t_philosophers;

typedef struct	s_main
{
	//................
	long			time;

	t_arg			args;
	t_mutex			mutexes;
	t_philosophers *p;
}				t_main;

#endif
