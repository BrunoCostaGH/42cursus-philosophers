/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 14:16:03 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/05/14 17:12:12 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	philo_sleep(t_master *master, int time_to_sleep, int id)
{
	t_philo	*philosopher;

	philosopher = master->philo_table[id - 1];
	sem_wait(philosopher->message_sem);
	print_message(philosopher, 3, id);
	sem_post(philosopher->message_sem);
	philosopher->is_sleeping = TRUE;
	wait_action(master, id, time_to_sleep, 0);
	philosopher->is_sleeping = FALSE;
}

void	philo_think(t_philo *philosopher, int id)
{
	sem_wait(philosopher->message_sem);
	print_message(philosopher, 4, id);
	sem_post(philosopher->message_sem);
	philosopher->is_thinking = TRUE;
}

void	*routine(void *arg)
{
	int			id;
	t_master	*master;
	t_philo		*philosopher;

	master = (t_master *)arg;
	id = master->philo_id;
	philosopher = master->philo_table[id - 1];
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

int	main(int argc, char **argv)
{
	t_master	*master;

	if ((argc == 5 || argc == 6) && is_valid(argv))
	{
		master = master_init(argv);
		proc_init(master);
		while (waitpid(master->philo_table[0]->philo_pid, NULL, 0) != -1)
			;
		free_semaphores(master);
		free_master(master);
	}
	else
		write(2, "Error!\nInvalid Arguments!\n", 26);
	return (0);
}
