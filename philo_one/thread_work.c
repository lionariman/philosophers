/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_work.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keuclide <keuclide@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/20 04:14:03 by keuclide          #+#    #+#             */
/*   Updated: 2021/05/21 06:22:25 by keuclide         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

void	*death_checking(void *philosopher)
{
	t_philosophers	*p;

	p = (t_philosophers *)philosopher;
	while (!p->args->died)
	{
		if ((time_stamp() - p->start - p->last) > p->args->time_to_die)
		{
			pthread_mutex_lock(&p->mutexes->m_write);
			if (p->args->died == 0)
				printf("%ld ms id %d died\n", time_stamp() - p->start, p->id);
			p->args->died = 1;
			pthread_mutex_unlock(&p->mutexes->m_write);
		}
	}
	return (NULL);
}

void	p_eating(t_philosophers *p)
{
	pthread_mutex_lock(p->l_fork);
	pthread_mutex_lock(p->r_fork);
	pthread_mutex_lock(&p->mutexes->m_write);
	if (p->args->died == 0)
		printf("%ld ms id %d has taken a forks\n",
			time_stamp() - p->start, p->id);
	p->last = time_stamp() - p->start;
	if (p->args->died == 0)
		printf("%ld ms id %d is eating\n", time_stamp() - p->start, p->id);
	pthread_mutex_unlock(&p->mutexes->m_write);
	m_sleep(p->args->time_to_eat);
	pthread_mutex_unlock(p->l_fork);
	pthread_mutex_unlock(p->r_fork);
}

void	p_sleeping(t_philosophers *p)
{
	pthread_mutex_lock(&p->mutexes->m_write);
	if (p->args->died == 0)
		printf("%ld ms id %d is sleeping\n", time_stamp() - p->start, p->id);
	pthread_mutex_unlock(&p->mutexes->m_write);
	m_sleep(p->args->time_to_sleep);
}

void	p_thinking(t_philosophers *p)
{
	pthread_mutex_lock(&p->mutexes->m_write);
	if (p->args->died == 0)
		printf("%ld ms id %d is thinking\n", time_stamp() - p->start, p->id);
	pthread_mutex_unlock(&p->mutexes->m_write);
}

int	p_filling(t_philosophers *p, int *i)
{
	if (p->args->num_of_times_each_p_must_eat > 0)
	{
		(*i)++;
		if (*i == p->args->num_of_times_each_p_must_eat)
		{
			if (p->args->died == 0)
			{
				pthread_mutex_lock(&p->mutexes->m_write);
				printf("%ld ms id %d is full\n", time_stamp() - p->start, p->id);
				pthread_mutex_unlock(&p->mutexes->m_write);
				return (1);
			}
		}
	}
	return (0);
}
