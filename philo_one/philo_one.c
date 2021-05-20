/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keuclide <keuclide@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/11 21:38:19 by keuclide          #+#    #+#             */
/*   Updated: 2021/05/20 04:20:29 by keuclide         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

void	*threads(void *philosopher)
{
	int				i;
	t_philosophers	*p;
	pthread_t		death_thread;

	i = 0;
	p = (t_philosophers *)philosopher;
	pthread_create(&death_thread, NULL, death_checking, (void *)p);
	pthread_detach(death_thread);
	while (p->args->died == 0)
	{
		p_eating(p);
		if (p_filling(p, &i))
			break ;
		p_sleeping(p);
		p_thinking(p);
	}
	return (NULL);
}

int	beginning(t_main *m)
{
	int	i;

	i = -1;
	while (++i < m->args->num_of_p)
	{
		if (pthread_create(&m->p[i].philosopher, NULL, threads, &m->p[i]))
			return (m_error("cannot create a philosopher"));
		m_sleep(1);
	}
	i = -1;
	while (++i < m->args->num_of_p)
		if (pthread_join(m->p[i].philosopher, NULL))
			return (m_error("cannot join a thread of philosopher"));
	return (0);
}

int	main(int ac, char **av)
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
	m.args->died = 1;
	m_free(&m);
	return (1);
}
