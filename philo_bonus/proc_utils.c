/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proc_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 18:05:36 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/04/19 19:25:02 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*kill_process(void *arg)
{
	t_master	*master;
	t_philo		*philosopher;

	master = (t_master *)arg;
	philosopher = master->philo_table[master->philo_id - 1];
	if (!philosopher->death_sem)
		philosopher->death_sem = sem_open(master->death_sem_name, 0);
	sem_wait(philosopher->death_sem);
	printf("Here bich\n");
	sem_close(philosopher->master_sem);
	sem_close(philosopher->fork_sem);
	sem_close(philosopher->death_sem);
	sem_close(philosopher->message_sem);
	pthread_detach(philosopher->thread_main);
	return (0);
}

static void	proc_thread_create(t_master *master, int id)
{
	master->philo_id = id;
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
	exit(1);
}

void	proc_init(t_master *master, int *supervisor)
{
	int		i;
	pid_t	pid;

	i = 0;
	while (i < master->number_of_philosophers)
	{
		pid = fork();
		if (!pid)
		{
			*supervisor = FALSE;
			proc_thread_create(master, i + 1);
			break ;
		}
		else
			master->philo_table[i]->philo_pid = pid;
		i++;
	}
}
