/*
 * think until the left fork is available; when it is, pick it up;
 * think until the right fork is available; when it is, pick it up;
 * when both forks are held, eat for a fixed amount of time;
 * then, put the right fork down;
 * then, put the left fork down;
 */

/*
 * what is thread and multithreading?
 * why should we use it?
 * what is mut_forks?
 * how to use it?
 * what is semaphore?
 * how to use it?
 */

#include "libft/libft.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>
#include <sys/time.h>
#include <sys/types.h>
#include <semaphore.h>
#include <signal.h>

#define DIE 1
#define EAT 2
#define SLEEP 3
#define	FULL 4
#define FORKS 5
#define THINK 6

// typedef	struct	s_semaphore
// {
// 	sem_t		*forks;
// }				t_semaphore;


typedef struct		s_arg
{
	int				num_of_p;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			time_to_stop;
}					t_arg;

typedef struct		s_philosophers
{
	pthread_t		philosopher;
	t_arg			arg;
	int				id;
	int				eat_up;
	long			time_td;
	long			start_time;
	long			last_eat_time;
}					t_philosophers;

int					died;
int					num_of_p;
long				start_time;
long				last_eat_time;
long				time_to_die;
long				time_to_eat;
long				time_to_sleep;
long				time_to_stop;

pid_t				*pid;

t_philosophers		*p;

char				**namebuf;
sem_t				**semaphore_block;

sem_t				*semaphore_forks;
sem_t				*semaphore_write;
sem_t				*semaphore_death;
sem_t				*semaphore_check;

int		m_strlen(char *line)
{
	int i;

	i = 0;
	while (line[i])
		i++;
	return (i);
}

int		m_putstr(char *line)
{
	return (write(1, line, m_strlen(line)));
}

int 	print_error(char *message)
{
	write(2, "Error: ", 7);
	write(2, message, m_strlen(message));
	write(2, "\n", 1);
	return (1);
}

int		m_isspace(char c)
{
	if (c == ' '|| c == '\t' || c == '\n'
	|| c == '\v'|| c == '\r' || c == '\f')
		return (1);
	return (0);
}

int		m_isdigit(char c)
{
	if (c > 47 && c < 58)
		return (1);
	return (0);
}

void	*free_maker(void **buffer)
{
	for (int i = 0; buffer[i] != '\0'; i++)
		free(buffer[i]);
	free(buffer);
	return (NULL);
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

// time
//--------------------------------------------------------------
long	time_stamp(void)
{
	struct	timeval	time;
	long	ret_val;

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
		usleep(amount_of_time_to_wait);
		current_time = time_stamp();
	}
}

//--------------------------------------------------------------

char	*get_block_name(char c, int n)
{
	char	*name;
	int		i;

	name = (char *)malloc(sizeof(char) * (n + 1));
	if (!name)
		return (NULL);
	i = -1;
	while (name[++i])
		name[i] = c;
	name[i] = '\0';
	return (name);
}

void	init_block_semaphores(void)
{
	namebuf = (char **)malloc(sizeof(char *) * num_of_p);
	semaphore_block = (sem_t **)malloc(sizeof(sem_t) * num_of_p);
	for (int i = 0; i < num_of_p; i++)
	{
		namebuf[i] = get_block_name('v', i + 1);
		sem_unlink(namebuf[i]);
		semaphore_block[i] = sem_open(namebuf[i], O_CREAT, 0644, 1);
	}	
}

void	m_message(t_philosophers *p, int value)
{
	sem_wait(semaphore_write);
	if (value == DIE)
		printf("%ld ms id %d died\n", time_stamp() - p->start_time, p->id);
	else if (value == EAT)
		printf("%ld ms id %d is eating\n", time_stamp() - p->start_time, p->id);
	else if (value == SLEEP)
		printf("%ld ms id %d is sleeping\n", time_stamp() - p->start_time, p->id);
	else if (value == THINK)
		printf("%ld ms id %d is thinking\n", time_stamp() - p->start_time, p->id);
	else if (value == FULL)
		printf("%ld ms id %d is full\n", time_stamp() - p->start_time, p->id);
	else if (value == FORKS)
		printf("%ld ms id %d has taken a forks\n", time_stamp() - p->start_time, p->id);
	sem_post(semaphore_write);
}

void	*death_check(void *philosopher)
{
	t_philosophers *p;

	p = (t_philosophers *)philosopher;
	while (!died)
	{
		sem_wait(semaphore_block[p->id - 1]);
		if (time_stamp() - p->start_time - p->last_eat_time > p->arg.time_to_die)
		{
			m_message(p, DIE);
			died = DIE;
			sem_post(semaphore_check);
			sem_post(semaphore_block[p->id - 1]);
			exit (0);
		}
		sem_post(semaphore_block[p->id - 1]);
	}
	return (NULL);
}

void	processes(t_philosophers *p)
{
	pthread_t	death_thread;

	pthread_create(&death_thread, NULL, death_check, (void *)p);
	pthread_detach(death_thread);
	while (!died)
	{
		sem_wait(semaphore_forks);
		m_message(p, FORKS);
		sem_wait(semaphore_block[p->id - 1]);
		p->last_eat_time = time_stamp() - p->start_time;
		m_message(p, EAT);
		m_sleep(p->arg.time_to_eat);
		sem_post(semaphore_block[p->id - 1]);
		sem_post(semaphore_forks);
		m_message(p, SLEEP);
		m_sleep(p->arg.time_to_sleep);
		m_message(p, THINK);
	}
}


//----- experimental part of code -----
void	*check_death_status_experimental(void *philosopher)
{
	t_philosophers *p;

	p = (t_philosophers *)philosopher;
	sem_wait(semaphore_death);
	// died = 1;
	return (NULL);
}
//-------------------------------------

void	beginning(t_philosophers *p)
{
	int i;
	//----- experimental part of code -----
	pthread_t	check_death_experimental;
	//-------------------------------------

	i = -1;
	while (++i < num_of_p)
	{
		pid[i] = fork();
		if (pid[i] == 0)
		{
			p->id = i + 1;
			processes(p);
		}
	}
	//----- experimental part of code -----
	pthread_create(&check_death_experimental, NULL, check_death_status_experimental, (void *)p);
	pthread_join(check_death_experimental, NULL);
	//-------------------------------------
	waitpid(-1, NULL, 0);
	i = -1;
	while (++i < num_of_p)
		kill(pid[i], SIGTERM);
}

int		main(int ac, char **av)
{
	int				i;
	t_philosophers	*p;

	if (ac < 5 || ac > 6)
	{
		print_error("wrong number of arguments");
		return (1);
	}
	died = 0;
	num_of_p = (int)m_atol(av[1]);
	time_to_die = m_atol(av[2]);
	time_to_eat = m_atol(av[3]);
	time_to_sleep = m_atol(av[4]);
	if (ac == 6)
		time_to_stop = m_atol(av[5]);


	sem_unlink("forks");
	semaphore_forks = sem_open("forks", O_CREAT, 0644, num_of_p / 2);
	sem_unlink("write");
	semaphore_write = sem_open("write", O_CREAT, 0644, 1);
	sem_unlink("death");
	semaphore_death = sem_open("death", O_CREAT, 0644, 0);
	sem_unlink("check");
	semaphore_check = sem_open("check", O_CREAT, 0644, 0);
	
	p = (t_philosophers *)malloc(sizeof(t_philosophers));
	p->start_time = time_stamp();
	p->last_eat_time = 0;
	p->arg.num_of_p = num_of_p;
	p->arg.time_to_die = time_to_die;
	p->arg.time_to_eat = time_to_eat;
	p->arg.time_to_sleep = time_to_sleep;

	//----------------------
	init_block_semaphores();
	pid = (pid_t *)malloc(sizeof(pid_t) * num_of_p);
	beginning(p);
	//----------------------
	died = 1;

	free(p);
	free(pid);
	sem_unlink("forks");
	sem_unlink("write");
	sem_unlink("death");
	sem_unlink("check");
	sem_close(semaphore_forks);
	sem_close(semaphore_write);
	sem_close(semaphore_death);
	sem_close(semaphore_check);
	// free_maker((void **)namebuf);
}
