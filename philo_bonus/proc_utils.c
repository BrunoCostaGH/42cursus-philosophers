/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proc_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 18:05:36 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/04/20 01:11:10 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*kill_process(void *arg)
{
	t_master	*master;
	t_philo		*philosopher;

	master = (t_master *)arg;
	philosopher = master->philo_table[master->philo_id - 1];
	sem_wait(philosopher->death_sem);
	philosopher->is_alive = FALSE;
	usleep(1000);
	sem_post(philosopher->master_sem);
	sem_post(philosopher->fork_sem);
	sem_post(philosopher->fork_sem);
	sem_post(philosopher->message_sem);
	while (philosopher->philo_pid != -1)
		;
	sem_close(philosopher->master_sem);
	sem_close(philosopher->fork_sem);
	sem_close(philosopher->death_sem);
	sem_close(philosopher->message_sem);
	return (0);
}

static void	proc_thread_create(t_master *master, int id)
{
	master->philo_id = id;
	philo_semaphores_init(master, id);
	if (pthread_create(&master->philo_table[id - 1]->thread_main, NULL, \
			&routine, master))
		return ;
	if (pthread_create(&master->philo_table[id - 1]->thread_vigilante, NULL, \
			&kill_process, master))
		return ;
	if (pthread_join(master->philo_table[id - 1]->thread_main, NULL))
		return ;
	if (pthread_join(master->philo_table[id - 1]->thread_vigilante, NULL))
		return ;
	free_master(master);
	exit(0);
}

int	proc_init(t_master *master)
{
	int		i;
	int		supervisor;
	pid_t	pid;

	i = 0;
	supervisor = TRUE;
	while (i < master->number_of_philosophers)
	{
		pid = fork();
		if (!pid)
		{
			supervisor = FALSE;
			proc_thread_create(master, i + 1);
			break ;
		}
		else
			master->philo_table[i]->philo_pid = pid;
		i++;
	}
	return (supervisor);
}
