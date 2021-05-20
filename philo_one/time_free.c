/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keuclide <keuclide@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/20 02:38:30 by keuclide          #+#    #+#             */
/*   Updated: 2021/05/20 04:15:35 by keuclide         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

void	m_free(t_main *m)
{
	free(m->args);
	free(m->mutexes->forks);
	free(m->mutexes);
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
