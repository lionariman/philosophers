/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_two.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keuclide <keuclide@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/12 08:57:21 by keuclide          #+#    #+#             */
/*   Updated: 2021/05/14 15:06:44 by keuclide         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

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

char	*m_memset_zero(char c, int n)
{
	char	*dest;
	int		i;

	dest = (char *)malloc(sizeof(char) * (n + 1));
	if (!dest)
		return (NULL);
	i = 0;
	while (i < n)
	{
		dest[i] = c;
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

void	*free_maker(char **buffer)
{
	int i;

	i = 0;
	while (buffer[i])
	{
		free(buffer[i]);
		i++;
	}
	free(buffer);
	return (NULL);
}

void	m_free(t_main *m)
{
	int i;

	sem_unlink("forks");
	sem_unlink("write");
	i = -1;
	while (++i < m->args.num_of_p)
	{
		sem_unlink(m->buff[i]);
		sem_close(m->semaphore->names[i]);
	}
	sem_close(m->semaphore->forks);
	sem_close(m->semaphore->s_write);
	free(m->semaphore->names);
	free(m->semaphore);
	free(m->p);
	free_maker(m->buff);
}

//************************************************************************

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

int		init_semaphores(t_main *m)
{
	m->semaphore = (t_semaphores *)malloc(sizeof(t_semaphores));
	if (!m->semaphore)
		return (m_error("cannot allocate memory for semaphores"));
	sem_unlink("forks");
	m->semaphore->forks = sem_open("forks", O_CREAT, 0644, m->args.num_of_p / 2);
	if (m->semaphore->forks == SEM_FAILED)
		return (m_error("cannot open semaphore for forks"));
	sem_unlink("write");
	m->semaphore->s_write = sem_open("write", O_CREAT, 0644, 1);
	if (m->semaphore->s_write == SEM_FAILED)
		return (m_error("cannot open semaphore for write"));
	return (0);
}

int		init_philosophers(t_main *m)
{
	long	start_time;
	int		i;

	m->p = (t_philosophers *)malloc(sizeof(t_philosophers) * m->args.num_of_p);
	if (!m->p)
		return (m_error("cannot allocate memory for philosophers"));
	start_time = time_stamp();
	i = -1;
	while (++i < m->args.num_of_p)
	{
		m->p[i].id = i + 1;
		m->p[i].semaphore = m->semaphore;
		m->p[i].args = m->args;
		m->p[i].start = start_time;
		m->p[i].last = 0;
	}
	return (0);
}

int		init_names(t_main *m)
{
	int		i;

	m->buff = (char **)malloc(sizeof(char *) * (m->args.num_of_p + 1));
	if (!m->buff)
		return (m_error("cannot allocate memory for **buff"));
	m->buff[m->args.num_of_p] = NULL;
	m->semaphore->names = (sem_t **)malloc(sizeof(sem_t *) * m->args.num_of_p);
	if (!m->semaphore->names)
		return (m_error("cannot allocate memory for names"));
	i = -1;
	while (++i < m->args.num_of_p)
	{
		m->buff[i] = m_memset_zero('x', i + 1);
		if (!m->buff[i])
			return (m_error("cannot allocate memory for *buff"));
		sem_unlink(m->buff[i]);
		m->semaphore->names[i] = sem_open(m->buff[i], O_CREAT, 0644, 1);
		if (m->semaphore->names[i] == SEM_FAILED)
			return (m_error("cannot open name"));
	}
	return (0);
}

//***************************************************************************

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
		sem_wait(p->semaphore->names[p->id - 1]);
		if (time_stamp() - p->start - p->last > p->args.time_to_die)
		{
			sem_wait(p->semaphore->s_write);
			printf("%ld ms id %d died\n", time_stamp() - p->start, p->id);
			died = 1;
			sem_post(p->semaphore->s_write);
		}
		sem_post(p->semaphore->names[p->id - 1]);
	}
	return NULL;
}


void	p_eating(t_philosophers *p)
{
	sem_wait(p->semaphore->forks);
	sem_wait(p->semaphore->s_write);
	if (died == 0)
		printf("%ld ms id %d has taken a fork\n", time_stamp() - p->start, p->id);
	sem_post(p->semaphore->s_write);
	p->last = time_stamp() - p->start;
	sem_wait(p->semaphore->names[p->id - 1]);
	sem_wait(p->semaphore->s_write);
	if (died == 0)
		printf("%ld ms id %d is eating\n", time_stamp() - p->start, p->id);
	sem_post(p->semaphore->s_write);
	m_sleep(p->args.time_to_eat);
	sem_post(p->semaphore->names[p->id - 1]);
	sem_post(p->semaphore->forks);
}

void	p_sleeping(t_philosophers *p)
{
	sem_wait(p->semaphore->s_write);
	if (died == 0)
		printf("%ld ms id %d is sleeping\n", time_stamp() - p->start, p->id);
	sem_post(p->semaphore->s_write);
	m_sleep(p->args.time_to_sleep);
}

void	p_thinking(t_philosophers *p)
{
	sem_wait(p->semaphore->s_write);
	if (died == 0)
		printf("%ld ms id %d is thinking\n", time_stamp() - p->start, p->id);
	sem_post(p->semaphore->s_write);
}

int		p_filling(t_philosophers *p, int *i)
{
	if (p->args.num_of_times_each_p_must_eat > 0)
	{
		(*i)++;
		if (*i == p->args.num_of_times_each_p_must_eat)
		{
			if (died == 0)
			{
				sem_wait(p->semaphore->s_write);
				printf("%ld ms id %d is full\n", time_stamp() - p->start, p->id);
				sem_post(p->semaphore->s_write);
				return (1);
			}
		}
	}
	return (0);
}

void	*threads(void *philosopher)
{
	int				i;
	t_philosophers	*p;
	pthread_t		death_thread;

	i = 0;
	p = (t_philosophers *)philosopher;
	pthread_create(&death_thread, NULL, death_checking, (void *)p);
	pthread_detach(death_thread);
	while (died == 0)
	{
		p_eating(p);
		if (p_filling(p, &i))
			break ;
		p_sleeping(p);
		p_thinking(p);
	}
	return NULL;
}

int		beginning(t_main *m)
{
	int			i;
	pthread_t	full_status_check;

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
	t_main m;

	if (ac < 5 || ac > 6)
	{
		m_error("wrong number of arguments");
		return (0);
	}
	if (validation(av))
		return (0);
	if (initialize(ac, av, &m))
		return (0);	
	if (init_semaphores(&m))
		return (0);
	if (init_philosophers(&m))
		return (0);
	if (init_names(&m))
		return (0);
	if (beginning(&m))
		return (0);
	m_free(&m);
	return (1);
}