/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_three.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keuclide <keuclide@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/14 15:08:14 by keuclide          #+#    #+#             */
/*   Updated: 2021/05/20 04:53:31 by keuclide         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

void	processes(t_philosophers *p)
{
	pthread_t	death_thread;
	int			i;

	pthread_create(&death_thread, NULL, death_checking, (void *)p);
	pthread_detach(death_thread);
	i = 0;
	while (!p->args->died)
	{
		p_eating(p);
		if (p_filling(p, &i))
			exit (0);
		p_sleeping(p);
		p_thinking(p);
	}
}

int	beginning(t_main *m)
{
	int	i;

	i = -1;
	while (++i < m->args->num_of_p)
	{
		m->p->pid[i] = fork();
		if (m->p->pid[i] == 0)
		{
			m->p->id = i + 1;
			processes(m->p);
		}
		else if (m->p->pid[i] == -1)
			return (m_error("cannot create a new process"));
	}
	waitpid(-1, NULL, 0);
	usleep(200000);
	i = -1;
	while (++i < m->args->num_of_p)
		kill(m->p->pid[i], SIGTERM);
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
	if (init_semaphores(&m))
		return (0);
	if (init_philosophers(&m))
		return (0);
	if (beginning(&m))
		return (0);
	m.args->died = 1;
	m_free(&m);
	return (0);
}
