/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 14:16:03 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/09/21 16:47:36 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	philo_sleep(t_master *master, unsigned int time_to_sleep, \
						unsigned int id)
{
	t_philo	*philosopher;

	philosopher = master->philo_table[id];
	sem_wait(philosopher->message_sem);
	print_message(philosopher, 3, id);
	sem_post(philosopher->message_sem);
	philosopher->is_sleeping = TRUE;
	wait_action(master, id, time_to_sleep, timestamp());
	philosopher->is_sleeping = FALSE;
}

void	philo_think(t_philo *philosopher, unsigned int id)
{
	sem_wait(philosopher->message_sem);
	print_message(philosopher, 4, id);
	sem_post(philosopher->message_sem);
	philosopher->is_thinking = TRUE;
}

void	*routine(void *arg)
{
	unsigned int	id;
	t_master		*master;
	t_philo			*philosopher;

	master = (t_master *)arg;
	id = master->philo_id;
	philosopher = master->philo_table[id];
	philosopher->time_to_die += timestamp();
	while (timestamp() <= philosopher->time_to_die && \
			access_philosopher_status(philosopher, 0))
	{
		go_to_table(master, id);
		if (philosopher->is_full && access_philosopher_status(philosopher, 0))
			philo_sleep(master, master->time_to_sleep, id);
	}
	kill_philosopher(master, id);
	return (0);
}

void	*the_master(t_master *master)
{
	unsigned int	i;

	i = 0;
	master->philo_table[0]->master_sem = sem_open(master->master_sem_name, 0);
	master->philo_table[0]->message_sem = sem_open(master->message_sem_name, 0);
	master->philo_table[0]->death_sem = sem_open(master->death_sem_name, 0);
	while (i < master->number_of_philosophers)
	{
		sem_wait(master->philo_table[0]->master_sem);
		i++;
	}
	while (i-- > 0)
		sem_post(master->philo_table[0]->death_sem);
	sem_close(master->philo_table[0]->master_sem);
	sem_close(master->philo_table[0]->message_sem);
	sem_close(master->philo_table[0]->death_sem);
	return (0);
}

int	main(int argc, char **argv)
{
	t_master	*master;
	pid_t		pid;

	if ((argc == 5 || argc == 6) && is_valid(argv))
	{
		master = master_init(argv);
		proc_init(master);
		pid = fork();
		if (pid == 0)
		{
			the_master(master);
			free_master(master);
			exit(0);
		}
		else
			while (waitpid(master->philo_table[1]->philo_pid, NULL, 0) != -1)
				;
		while (waitpid(-1, NULL, 0) != -1)
			;
		free_semaphores(master);
		free_master(master);
	}
	else
		write(2, "Error!\nInvalid Arguments!\n", 26);
	return (0);
}
