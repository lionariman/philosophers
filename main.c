#include "philosophers.h"

/*
• One fork between each philosopher, therefore there will be a fork at the right and at the left of each philosopher.
• To avoid philosophers duplicating forks, you should protect the forks state with a mutex for each of them.
• Each philosopher should be a thread.
*/

int 	m_strlen(char *line)
{
	int i;

	i = 0;
	while (line[i])
		i++;
	return (i);
}

int 	print_error(char *message)
{
	write(2, "Error: ", 7);
	write(2, message, m_strlen(message));
	return (1);
}

int		m_isdigit(char c)
{
	if (c > 47 && c < 58)
		return (1);
	return (0);
}

int		m_isspace(char c)
{
	if (c == ' '|| c == '\t' || c == '\n'
	|| c == '\v'|| c == '\r' || c == '\f')
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

void	free_memory(t_main *m)
{
	free(m->mutexes.forks);
	free(m->p);
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------

int		validation(char **av)
{
	int i;
	int	j;

	i = 0;
	while (av[++i])
	{
		j = -1;
		while (av[i][++j])
		{
			if (!m_isdigit(av[i][j]))
				return (print_error("not valid argument\n"));
		}
	}
	return (0);
}

int		initialize(char ac, char **av, t_main *m)
{
	m->args.num_of_p = m_atol(av[1]);
	if (m->args.num_of_p < 2 || m->args.num_of_p > 200)
		return (print_error("wrong number of philosophers\n"));
	m->args.time_to_die = m_atol(av[2]);
	m->args.time_to_eat = m_atol(av[3]);
	m->args.time_to_sleep = m_atol(av[4]);
	m->args.num_of_times_each_p_must_eat = 0;
	if (ac == 6)
		m->args.num_of_times_each_p_must_eat = m_atol(av[4]);
	return (0);
}

int		init_mutexes(t_main *m)
{
	int i;

	m->mutexes.forks =
	(pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * m->args.num_of_p);
	if (!m->mutexes.forks)
		return (1);
	i = -1;
	while (++i < m->args.num_of_p)
		pthread_mutex_init(&m->mutexes.forks[i], NULL);
	// pthread_mutex_init(&m->mutexes.died, NULL);
	// pthread_mutex_init(&m->mutexes.mstat, NULL);
	return (0);
}

// long	save_time(long time)
// {
// 	struct timeval	the_time;

// 	gettimeofday(&the_time, NULL);
// 	time = the_time.tv_sec * 1000;
// 	time += the_time.tv_usec / 1000;
// 	return (time);
// }

int		init_philosophers(t_main *m)
{
	int		i;

	// m->time = save_time(m->time);
	i = -1;
	m->p = (t_philosophers *)malloc(sizeof(t_philosophers) * m->args.num_of_p);
	if (!m->p)
		return (print_error("cannot allocate memory: t_philosophers"));
	while (++i < m->args.num_of_p)
	{
		m->p[i].id = i + 1;
		m->p[i].l_fork = m->mutexes.forks[i];
		if (i == m->args.num_of_p)
			m->p[i].r_fork = m->mutexes.forks[0];
		else
			m->p[i].r_fork = m->mutexes.forks[i + 1];
		// m->p->start = m->time;
		// m->p->last = 0;
		m->p->args = m->args;
		m->p->mutexes = m->mutexes;
	}
	return (0);
}

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------

void	*simulation(void *philosopher)
{
	int	i;
	t_philosophers *p;

	p = philosopher;
	i = -1;
	pthread_mutex_lock(&p->l_fork);
	pthread_mutex_lock(&p->r_fork);
	printf("philosopher %d take forks\n", p->id);
	usleep(2000000);
	printf("philosopher %d is eating\n", p->id);
	usleep(2000000);
	printf("philosopher %d left forks\n", p->id);
	pthread_mutex_unlock(&p->l_fork);
	pthread_mutex_unlock(&p->r_fork);
	return NULL;
}

int		beginning(t_main *m)
{
	int i;

	i = -1;
	while (++i < m->args.num_of_p)
	{
		printf("%d philosopher was born\n", m->p[i].id);
		pthread_create(&m->p[i].thread, NULL, simulation, &m->p[i]);
		sleep(1);
	}
	//...............
	//...............
	//...............
	i = -1;
	while (++i < m->p->args.num_of_p)
		pthread_join(m->p[i].thread, NULL);
	return (0);
}

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------

int 	main(int ac, char **av)
{
	t_main	m;

	if (ac < 5 || ac > 6)
		return (print_error("wrong number of arguments.\n"));
	if (validation(av))
		return (1);
	if (initialize(ac, av, &m))
		return (1);
	if (init_mutexes(&m))
		return (1);
	if (init_philosophers(&m))
		return (1);
	if (beginning(&m))
		return (1);
	free_memory(&m);
	// while (1);
	return (0);
}