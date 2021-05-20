/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keuclide <keuclide@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/20 04:39:55 by keuclide          #+#    #+#             */
/*   Updated: 2021/05/20 04:41:28 by keuclide         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

void	m_free(t_main *m)
{
	sem_unlink("forks");
	sem_unlink("write");
	sem_close(m->semaphore->forks);
	sem_close(m->semaphore->s_write);
	free(m->pid);
	free(m->args);
	free(m->semaphore);
	free(m->p);
}

long	time_stamp(void)
{
	struct timeval	time;
	long			ret_val;

	gettimeofday(&time, NULL);
	ret_val = time.tv_sec * 1000 + time.tv_usec / 1000;
	return (ret_val);
}

void	m_sleep(long amount_of_time_to_wait)
{
	long	current_time;
	long	period_of_time;

	current_time = time_stamp();
	period_of_time = current_time + amount_of_time_to_wait;
	while (period_of_time > current_time)
	{
		usleep(1);
		current_time = time_stamp();
	}
}
