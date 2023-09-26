/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proc_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 18:05:36 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/09/26 18:39:04 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*kill_thread(void *arg)
{
	t_master	*master;
	t_philo		*philosopher;

	master = (t_master *)arg;
	philosopher = master->philo_table[master->philo_id];
	sem_wait(philosopher->death_sem);
	access_philosopher_status(philosopher, 1);
	sem_post(philosopher->m_fork_sem);
	sem_post(philosopher->fork_sem);
	usleep(1000);
	sem_post(philosopher->message_sem);
	return (0);
}

static void	*fork_thread(void *arg)
{
	t_master	*master;
	t_philo		*philosopher;

	master = (t_master *)arg;
	philosopher = master->philo_table[master->philo_id];
	if (master->philo_id % 2 == 0)
		usleep(100);
	while (access_philosopher_status(philosopher, 0))
	{
		sem_wait(philosopher->vigilante_sem);
		if (philosopher->has_forks != 2 && !philosopher->is_full)
		{
			sem_post(philosopher->vigilante_sem);
			check_fork_status(philosopher, master->philo_id);
			sem_wait(philosopher->vigilante_sem);
		}
		sem_post(philosopher->vigilante_sem);
	}
	return (0);
}

static void	philo_semaphores_init(t_master *master, unsigned int id)
{
	t_philo	*philosopher;

	philosopher = master->philo_table[id];
	philosopher->m_fork_sem = sem_open(master->m_fork_sem_name, 0);
	philosopher->fork_sem = sem_open(master->fork_sem_name, 0);
	philosopher->message_sem = sem_open(master->message_sem_name, 0);
	philosopher->death_sem = sem_open(master->death_sem_name, 0);
	philosopher->vigilante_sem = sem_open(master->vigilante_sem_name, 0);
	philosopher->master_sem = sem_open(master->master_sem_name, 0);
	if (!philosopher->fork_sem || !philosopher->message_sem || \
		!philosopher->m_fork_sem || !philosopher->death_sem || \
		!philosopher->vigilante_sem || !philosopher->master_sem)
	{
		printf("\e[1;41m===%d===ERROR: semaphore failed on open\e[0m\n", id);
		free_master(master);
		exit(1);
	}
}

static void	proc_thread_create(t_master *master, unsigned int id)
{
	philo_semaphores_init(master, id);
	timestamp();
	if (id && pthread_create(&master->philo_table[id]->thread_main, NULL, \
	&routine, master))
		id = 0;
	if (id && pthread_create(&master->philo_table[id]->thread_forks, NULL, \
	&fork_thread, master))
		id = 0;
	if (id && pthread_create(&master->philo_table[id]->thread_vigilante, \
	NULL, &kill_thread, master))
		id = 0;
	if (id && pthread_join(master->philo_table[id]->thread_main, NULL))
		id = 0;
	if (id && pthread_join(master->philo_table[id]->thread_forks, NULL))
		id = 0;
	if (id && pthread_join(master->philo_table[id]->thread_vigilante, NULL))
		id = 0;
	sem_close(master->philo_table[master->philo_id]->m_fork_sem);
	sem_close(master->philo_table[master->philo_id]->fork_sem);
	sem_close(master->philo_table[master->philo_id]->message_sem);
	sem_close(master->philo_table[master->philo_id]->death_sem);
	sem_close(master->philo_table[master->philo_id]->vigilante_sem);
	sem_close(master->philo_table[master->philo_id]->master_sem);
	free_master(master);
	exit(0);
}

void	proc_init(t_master *master)
{
	unsigned int	i;
	pid_t			pid;

	i = 1;
	while (i < master->number_of_philosophers + 1)
	{
		pid = fork();
		if (!pid)
		{
			master->philo_id = i;
			proc_thread_create(master, master->philo_id);
		}
		else
			master->philo_table[i]->philo_pid = pid;
		i++;
	}
}
