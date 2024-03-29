/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 21:18:23 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/05/14 17:03:24 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
/*
Your(s) program(s) should take the following arguments:

number_of_philosophers time_to_die time_to_eat time_to_sleep
[number_of_times_each_philosopher_must_eat]

◦ number_of_philosophers: The number of philosophers and also the number
of forks.

◦ time_to_die (in milliseconds): If a philosopher did not start eating
time_to_die milliseconds since the beginning of their last meal or the 
beginning of the simulation, they die.

◦ time_to_eat (in milliseconds): The time it takes for a philosopher to eat.
During that time, they will need to hold two forks.

◦ time_to_sleep (in milliseconds): The time a philosopher will spend sleeping.

◦ number_of_times_each_philosopher_must_eat (optional argument): If all
philosophers have eaten at least number_of_times_each_philosopher_must_eat
times, the simulation stops. If not specified, the simulation stops when a
philosopher dies.

• Each philosopher has a number ranging from 1 to number_of_philosophers.
• Philosopher number 1 sits next to philosopher number number_of_philosophers.
Any other philosopher number N sits between philosopher number N - 1 and 
philosopher number N + 1.
*/

static void	philo_sleep(t_master *master, unsigned int id)
{
	unsigned int	time_to_sleep;
	t_philo			*philosopher;

	time_to_sleep = master->time_to_sleep;
	philosopher = master->philo_table[id - 1];
	pthread_mutex_lock(&master->mutex_message);
	print_message(master, 3, id);
	philosopher->is_eating = FALSE;
	philosopher->is_sleeping = TRUE;
	pthread_mutex_unlock(&master->mutex_message);
	wait_action(master, id, time_to_sleep, timestamp(master));
	philo_think(master, id);
	wait_action(master, id, 5, timestamp(master));
}

void	philo_think(t_master *master, unsigned int id)
{
	t_philo		*philosopher;

	philosopher = master->philo_table[id - 1];
	pthread_mutex_lock(&master->mutex_message);
	print_message(master, 4, id);
	philosopher->is_thinking = TRUE;
	pthread_mutex_unlock(&master->mutex_message);
}

void	*routine(void *arg)
{
	unsigned int	id;
	t_master		*master;

	master = (t_master *)arg;
	pthread_mutex_lock(&master->mutex_routine);
	id = ++master->philo_id;
	pthread_mutex_unlock(&master->mutex_routine);
	master->philo_table[id - 1]->time_to_die += timestamp(master);
	while (!check_simulation_status(master))
	{
		go_to_table(master, id);
		if (master->philo_table[id - 1]->is_eating \
			&& !check_simulation_status(master))
			philo_sleep(master, id);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	unsigned int	i;
	t_master		*master;

	if ((argc == 5 || argc == 6) && is_valid(argv))
	{
		i = 0;
		master = master_init(argv);
		while (i < master->number_of_philosophers)
		{
			if (pthread_create(&master->philo_table[i++]->thread, NULL, \
				&routine, master))
				break ;
		}
		i = 0;
		while (i < master->number_of_philosophers)
			if (pthread_join(master->philo_table[i++]->thread, NULL))
				break ;
		free_master(master);
	}
	else
		write(2, "Error!\nInvalid Arguments!\n", 26);
	return (0);
}
