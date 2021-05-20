/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_work.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keuclide <keuclide@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/20 04:25:39 by keuclide          #+#    #+#             */
/*   Updated: 2021/05/20 04:34:22 by keuclide         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

void	*death_checking(void *philosopher)
{
	t_philosophers	*p;

	p = (t_philosophers *)philosopher;
	while (p->args->died == 0)
	{
		if (time_stamp() - p->start - p->last > p->args->time_to_die)
		{
			sem_wait(p->semaphore->s_write);
			if (p->args->died == 0)
				printf("%ld ms id %d died\n", time_stamp() - p->start, p->id);
			p->args->died = 1;
			sem_post(p->semaphore->s_write);
		}
	}
	return (NULL);
}

void	p_eating(t_philosophers *p)
{
	sem_wait(p->semaphore->forks);
	sem_wait(p->semaphore->s_write);
	if (p->args->died == 0)
		printf("%ld ms id %d has taken a forks\n",
			time_stamp() - p->start, p->id);
	sem_post(p->semaphore->s_write);
	sem_wait(p->semaphore->s_write);
	if (p->args->died == 0)
		printf("%ld ms id %d is eating\n", time_stamp() - p->start, p->id);
	sem_post(p->semaphore->s_write);
	p->last = time_stamp() - p->start;
	m_sleep(p->args->time_to_eat);
	sem_post(p->semaphore->forks);
}

void	p_sleeping(t_philosophers *p)
{
	sem_wait(p->semaphore->s_write);
	if (p->args->died == 0)
		printf("%ld ms id %d is sleeping\n", time_stamp() - p->start, p->id);
	sem_post(p->semaphore->s_write);
	m_sleep(p->args->time_to_sleep);
}

void	p_thinking(t_philosophers *p)
{
	sem_wait(p->semaphore->s_write);
	if (p->args->died == 0)
		printf("%ld ms id %d is thinking\n", time_stamp() - p->start, p->id);
	sem_post(p->semaphore->s_write);
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
				sem_wait(p->semaphore->s_write);
				printf("%ld ms id %d is full\n", time_stamp() - p->start, p->id);
				sem_post(p->semaphore->s_write);
				return (1);
			}
		}
	}
	return (0);
}
