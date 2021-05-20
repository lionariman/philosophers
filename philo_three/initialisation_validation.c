/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialisation_validation.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keuclide <keuclide@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/20 04:41:58 by keuclide          #+#    #+#             */
/*   Updated: 2021/05/20 04:53:50 by keuclide         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

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

int	init_semaphores(t_main *m)
{
	m->semaphore = (t_semaphores *)malloc(sizeof(t_semaphores));
	if (!m->semaphore)
		return (m_error("cannot allocate memory for struct of semaphores"));
	sem_unlink("forks");
	m->semaphore->forks = sem_open("forks",
			O_CREAT, 0644, m->args->num_of_p / 2);
	if (m->semaphore->forks == SEM_FAILED)
		return (m_error("forks semaphor open failed"));
	sem_unlink("write");
	m->semaphore->s_write = sem_open("write", O_CREAT, 0644, 1);
	if (m->semaphore->s_write == SEM_FAILED)
		return (m_error("s_write semaphor open failed"));
	return (0);
}

int	init_philosophers(t_main *m)
{
	m->p = (t_philosophers *)malloc(sizeof(t_philosophers));
	if (!m->p)
		return (m_error("cannot allocate memory for philosophers struct"));
	m->pid = (pid_t *)malloc(sizeof(pid_t) * m->args->num_of_p);
	if (!m->pid)
		return (m_error("cannot allocate memory for pid"));
	m->p->args = m->args;
	m->p->semaphore = m->semaphore;
	m->p->start = time_stamp();
	m->p->last = 0;
	m->p->pid = m->pid;
	return (0);
}
