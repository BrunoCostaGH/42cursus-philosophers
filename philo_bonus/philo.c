/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 14:16:03 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/04/17 19:56:36 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	philo_sleep(t_philo *philosopher, int time_to_sleep, int id)
{
	sem_wait(philosopher->message_sem);
	print_message(3, id);
	sem_post(philosopher->message_sem);
	philosopher->is_sleeping = TRUE;
	wait_action(philosopher, id, time_to_sleep);
	philosopher->is_sleeping = FALSE;
	philosopher->is_full = FALSE;
}

static void	philo_think(t_philo *philosopher, int id)
{
	sem_wait(philosopher->message_sem);
	print_message(4, id);
	sem_post(philosopher->message_sem);
	philosopher->is_thinking = TRUE;
}

static void	philo_eat(t_philo *philosopher, int time_to_eat, int id)
{
	while (timestamp() < philosopher->time_to_die)
	{
		check_fork_status(philosopher, id);
		if (philosopher->has_forks == 2)
		{
			sem_wait(philosopher->message_sem);
			print_message(2, id);
			sem_post(philosopher->message_sem);
			philosopher->is_thinking = FALSE;
			philosopher->is_eating = TRUE;
			wait_action(philosopher, id, time_to_eat);
			clean_the_forks(philosopher);
			philosopher->is_eating = FALSE;
			philosopher->is_full = TRUE;
			philosopher->number_of_times_has_eaten++;
			return ;
		}
		else if (philosopher->is_thinking == FALSE)
			philo_think(philosopher, id);
	}
	sem_wait(philosopher->death_sem);
	kill_philosopher(philosopher, id);
}

void	routine(t_master *master, int id)
{
	t_philo	*philosopher;

	philosopher = master->philo_table[id - 1];
	philosopher->time_to_die += timestamp();
	philo_semaphores_init(master, id);
	while (timestamp() < philosopher->time_to_die)
	{
		if (master->number_of_philosophers > 1)
		{
			philo_eat(philosopher, master->time_to_eat, id);
			philosopher->time_to_die = timestamp() + master->time_to_die;
		}
		if (philosopher->is_full)
			philo_sleep(philosopher, master->time_to_sleep, id);
		else if (philosopher->is_thinking == FALSE)
			philo_think(philosopher, id);
	}
	sem_wait(philosopher->death_sem);
	kill_philosopher(philosopher, id);
}

int	main(int argc, char **argv)
{
	int			supervisor;
	t_master	*master;

	if ((argc == 5 || argc == 6))
	{
		supervisor = TRUE;
		master = master_init(argv);
		proc_init(master, &supervisor);
		if (supervisor)
		{
			waitpid(-1, NULL, 0);
			spectate(master);
		}
		free_master(master);
	}
	return (0);
}
