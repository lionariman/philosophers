/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keuclide <keuclide@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/11 21:38:19 by keuclide          #+#    #+#             */
/*   Updated: 2021/05/12 08:50:26 by keuclide         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int		m_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int		m_error(char *message)
{
	write(2, "Error: ", 7);
	write(2, message, m_strlen(message));
	write(2, "\n", 1);
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

void	m_free(t_main *m)
{
	free(m->p);
	free(m->mutexes.forks);
}

//*****************************************************************************

long	time_stamp(void)
{
	struct	timeval	time;
	long	ret_val;

	gettimeofday(&time, NULL);
	ret_val = time.tv_sec * 1000 + time.tv_usec / 1000;
	return (ret_val);
}

int		validation(char **av)
{
	int i;
	int j;

	i = 0;
	while (av[++i])
	{
		j = -1;
		while (av[i][++j])
		{
			if (!m_isdigit(av[i][j]))
				return (m_error("not valid argument"));
		}
	}
	return (0);
}

int		initialize(int ac, char **av, t_main *m)
{
	died = 0;
	m->args.num_of_p = m_atol(av[1]);
	if (m->args.num_of_p < 2 || m->args.num_of_p > 200)
		return (m_error("wrong number of philosophers"));
	m->args.time_to_die = m_atol(av[2]);
	m->args.time_to_eat = m_atol(av[3]);
	m->args.time_to_sleep = m_atol(av[4]);
	if (ac == 6)
		m->args.num_of_times_each_p_must_eat = m_atol(av[5]);
	else
		m->args.num_of_times_each_p_must_eat = 0;
	return (0);
}

int		init_mutexes(t_main *m)
{
	int i;
	m->mutexes.forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * m->args.num_of_p);
	if (!m->mutexes.forks)
		return (m_error("cannot allocate memory for forks"));
	i = -1;
	while (++i < m->args.num_of_p)
		if (pthread_mutex_init(&m->mutexes.forks[i], NULL))
			return (m_error("cannot initialize mutex for forks"));
	if (pthread_mutex_init(&m->mutexes.m_died, NULL))
		return (m_error("cannot initialize mutex for m_died"));
	if (pthread_mutex_init(&m->mutexes.m_write, NULL))
		return (m_error("cannot initialize mutex for m_write"));
	return (0);
}

int		init_philosophers(t_main *m)
{
	int i;

	i = -1;
	m->p = (t_philosophers *)malloc(sizeof(t_philosophers) * m->args.num_of_p);
	if (!m->p)
		return (m_error("cannot allocate memory for philosophers"));
	while (++i < m->args.num_of_p)
	{
		m->p[i].id = i + 1;
		m->p[i].l_fork = &m->mutexes.forks[i];
		m->p[i].r_fork = &m->mutexes.forks[(i + 1) % m->args.num_of_p];
		m->p[i].mutexes = m->mutexes;
		m->p[i].args = m->args;
		m->p[i].start = time_stamp();
		m->p[i].last = 0;
	}
	return (0);
}

//***********************************************************************************

void	m_sleep(long time)
{
	long ts;

	ts = time_stamp();
	while (time_stamp() - ts < time)
		usleep(1);
}

void	*death_checking(void *philosopher)
{
	t_philosophers	*p;

	p = (t_philosophers *)philosopher;
	while (died == 0)
	{
		pthread_mutex_lock(&p->mutexes.m_wait);
		if ((time_stamp() - p->start - p->last) > p->args.time_to_die)
		{
			pthread_mutex_lock(&p->mutexes.m_write);
			if (died == 0)
				printf("%ld ms id %d died\n", time_stamp() - p->start, p->id);
			pthread_mutex_unlock(&p->mutexes.m_write);
			pthread_mutex_lock(&p->mutexes.m_died);
			died = 1;
			pthread_mutex_unlock(&p->mutexes.m_died);
			pthread_mutex_unlock(&p->mutexes.m_wait);
			break ;
		}
		pthread_mutex_unlock(&p->mutexes.m_wait);
	}
	return NULL;
}

void	p_eating(t_philosophers *p)
{
	pthread_mutex_lock(p->l_fork);
	pthread_mutex_lock(p->r_fork);
	pthread_mutex_lock(&p->mutexes.m_write);
	if (died == 0)
		printf("%ld ms id %d has taken a fork\n", time_stamp() - p->start, p->id);
	pthread_mutex_unlock(&p->mutexes.m_write);
	p->last = time_stamp() - p->start;
	pthread_mutex_lock(&p->mutexes.m_wait);
	pthread_mutex_lock(&p->mutexes.m_write);
	if (died == 0)
		printf("%ld ms id %d is eating\n", time_stamp() - p->start, p->id);
	pthread_mutex_unlock(&p->mutexes.m_write);
	m_sleep(p->args.time_to_eat);
	pthread_mutex_unlock(&p->mutexes.m_wait);
	pthread_mutex_unlock(p->l_fork);
	pthread_mutex_unlock(p->r_fork);
}

void	p_sleeping(t_philosophers *p)
{
	pthread_mutex_lock(&p->mutexes.m_write);
	if (died == 0)
		printf("%ld ms id %d is sleeping\n", time_stamp() - p->start, p->id);
	pthread_mutex_unlock(&p->mutexes.m_write);
	m_sleep(p->args.time_to_sleep);
}

void	p_thinking(t_philosophers *p)
{
	pthread_mutex_lock(&p->mutexes.m_write);
	if (died == 0)
		printf("%ld ms id %d is thinking\n", time_stamp() - p->start, p->id);
	pthread_mutex_unlock(&p->mutexes.m_write);
}

void	p_filling(t_philosophers *p, int *i)
{
	if (p->args.num_of_times_each_p_must_eat > 0)
	{
		(*i)++;
		if (*i == p->args.num_of_times_each_p_must_eat)
		{
			if (died == 0)
			{
				pthread_mutex_lock(&p->mutexes.m_write);
				printf("%ld ms id %d is full\n", time_stamp() - p->start, p->id);
				pthread_mutex_unlock(&p->mutexes.m_write);
				return ;
			}
		}
	}
}

void	*threads(void *philosopher)
{
	int				i;
	t_philosophers	*p;
	pthread_t		death_thread;

	i = 0;
	p = (t_philosophers *)philosopher;
	pthread_create(&death_thread, NULL, death_checking, (void *)p);
	while (died == 0)
	{
		p_eating(p);
		p_filling(p, &i);
		p_sleeping(p);
		p_thinking(p);
	}
	return NULL;
}

int		beginning(t_main *m)
{
	int i;

	i = -1;
	while (++i < m->args.num_of_p)
	{
		if (pthread_create(&m->p[i].philosopher, NULL, threads, &m->p[i]))
			return (m_error("cannot create a philosopher"));
		m_sleep(10);
	}
	i = -1;
	while (++i < m->args.num_of_p)
		if (pthread_join(m->p[i].philosopher, NULL))
			return (m_error("cannot join a thread of philosopher"));
	return (0);
}

int		main(int ac, char **av)
{
	t_main	m;

	if (ac < 5 || ac > 6)
	{
		m_error("wrong number of arguments");
		return (0);
	}
	if (validation(av))
		return (0);
	if (initialize(ac, av, &m))
		return (0);
	if (init_mutexes(&m))
		return (0);
	if (init_philosophers(&m))
		return (0);
	if (beginning(&m))
		return (0);
	m_free(&m);
	return (1);
}