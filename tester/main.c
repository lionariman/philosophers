/*
 * think until the left fork is available; when it is, pick it up;
 * think until the right fork is available; when it is, pick it up;
 * when both forks are held, eat for a fixed amount of time;
 * then, put the right fork down;
 * then, put the left fork down;
 */

/*
 * what is thread and multithreading?
 * why should we use it?
 * what is mut_forks?
 * how to use it?
 * what is semaphore?
 * how to use it?
 */

#include "libft/libft.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>
#include <sys/time.h>

#include <sys/types.h>

#define LEFT (i - 1) % 5
#define RIGHT (i + 1) % 5
#define HUNGARY 0
#define EATING 1
#define THINKING 2

typedef struct		s_philosophers
{
	pthread_t		philosopher;
	int				id;
	pthread_mutex_t	l_fork;
	pthread_mutex_t	r_fork;
}					t_philosophers;

int					num_of_p;
int					num_of_food;
pthread_mutex_t		*mut_forks;
t_philosophers		*p;

int		m_strlen(char *line)
{
	int i;

	i = 0;
	while (line[i])
		i++;
	return (i);
}

int		m_putstr(char *line)
{
	return (write(1, line, m_strlen(line)));
}

int 	print_error(char *message)
{
	write(2, "Error: ", 7);
	write(2, message, m_strlen(message));
	write(2, "\n", 1);
	return (1);
}

int		m_isspace(char c)
{
	if (c == ' '|| c == '\t' || c == '\n'
	|| c == '\v'|| c == '\r' || c == '\f')
		return (1);
	return (0);
}

int		m_isdigit(char c)
{
	if (c > 47 && c < 58)
		return (1);
	return (0);
}

long	m_atol(char *number)
{
	long	l;
	int		sign;

	sign = 1;
	while (*number && m_isspace(*number))
		number++;
	if (*number == '-')
		sign = -1;
	if (*number == '-' || *number == '+')
		number++;
	l = 0;
	while (*number && m_isdigit(*number))
	{
		l = l * 10 + (*number - 48);
		number++;
	}
	if (l > INT_MAX || l < INT_MIN)
		return (LONG_MAX);
	return (l * sign);
}

void	*simulation(void *philosopher)
{
	int		i;
	long	time;
	t_philosophers *p;

	p = philosopher;
	i = -1;
	pthread_mutex_lock(&p->l_fork);
	pthread_mutex_lock(&p->r_fork);
	printf("philosopher %d take forks\n", p->id);
	usleep(2000000);
	printf("philosopher %d is EATING\n", p->id);
	usleep(2000000);
	printf("philosopher %d left forks\n", p->id);
	pthread_mutex_unlock(&p->l_fork);
	pthread_mutex_unlock(&p->r_fork);
	return NULL;
}

int		main(int ac, char **av)
{
	int				i;
	t_philosophers	*p;

	if (ac != 2)
	{
		print_error("wrong number of arguments");
		return (1);
	}
	num_of_p = (int)m_atol(av[1]);
	p = (t_philosophers *)malloc(sizeof(t_philosophers) * num_of_p);
	mut_forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * num_of_p);

	// forks (mutexes) initialisation for philosophers
	i = -1;
	while (++i < num_of_p)
		pthread_mutex_init(&mut_forks[i], NULL);

	// initialisation of philosophers
	i = -1;
	while (++i < num_of_p)
	{
		p[i].id = i + 1;
		p[i].l_fork = mut_forks[i];
		if (i == num_of_p)
			p[i].r_fork = mut_forks[0];
		else
			p[i].r_fork = mut_forks[i + 1];
	}

	// initialisation of simulation
	i = -1;
	while (++i < num_of_p)
	{
		printf("philosopher %d was born\n", p[i].id);
		pthread_create(&p[i].philosopher, NULL, simulation, &p[i]);
		// usleep(1000000);
		sleep(1);
	}

	// waiting each philosopher (thread) until it completed
	i = -1;
	while (++i < num_of_p)
		pthread_join(p[i].philosopher, NULL);
	free(p);
	free(mut_forks);
	// usleep(1000);
	// while (1);
}
