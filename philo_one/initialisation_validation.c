/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialisation_validation.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keuclide <keuclide@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/20 02:33:55 by keuclide          #+#    #+#             */
/*   Updated: 2021/05/20 04:10:52 by keuclide         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int	validation(char **av)
{
	int	i;
	int	j;

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

int	initialize(int ac, char **av, t_main *m)
{
	m->args = (t_arg *)malloc(sizeof(t_arg));
	if (!m->args)
		return (m_error("cannot allocate memory for arguments"));
	m->args->died = 0;
	m->args->num_of_p = m_atol(av[1]);
	if (m->args->num_of_p < 2 || m->args->num_of_p > 200)
		return (m_error("wrong number of philosophers"));
	m->args->time_to_die = m_atol(av[2]);
	m->args->time_to_eat = m_atol(av[3]);
	m->args->time_to_sleep = m_atol(av[4]);
	if (ac == 6)
		m->args->num_of_times_each_p_must_eat = m_atol(av[5]);
	else
		m->args->num_of_times_each_p_must_eat = 0;
	return (0);
}

int	init_mutexes(t_main *m)
{
	int	i;

	m->mutexes = (t_mutex *)malloc(sizeof(t_mutex));
	if (!m->mutexes)
		return (m_error("cannot allocate memory for mutexes"));
	m->mutexes->forks = (pthread_mutex_t *)malloc(
			sizeof(pthread_mutex_t) * m->args->num_of_p);
	if (!m->mutexes->forks)
		return (m_error("cannot allocate memory for forks"));
	i = -1;
	while (++i < m->args->num_of_p)
		if (pthread_mutex_init(&m->mutexes->forks[i], NULL))
			return (m_error("cannot initialize mutex for forks"));
	if (pthread_mutex_init(&m->mutexes->m_write, NULL))
		return (m_error("cannot initialize mutex for m_write"));
	return (0);
}

int	init_philosophers(t_main *m)
{
	long	start_time;
	int		i;

	i = -1;
	m->p = (t_philosophers *)malloc(sizeof(t_philosophers) * m->args->num_of_p);
	if (!m->p)
		return (m_error("cannot allocate memory for philosophers"));
	start_time = time_stamp();
	while (++i < m->args->num_of_p)
	{
		m->p[i].id = i + 1;
		m->p[i].l_fork = &m->mutexes->forks[i];
		m->p[i].r_fork = &m->mutexes->forks[(i + 1) % m->args->num_of_p];
		m->p[i].mutexes = m->mutexes;
		m->p[i].args = m->args;
		m->p[i].start = start_time;
		m->p[i].last = 0;
	}
	return (0);
}
