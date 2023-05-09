/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_eat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 15:12:05 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/05/09 20:01:55 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	philo_eat(t_master *master, int id)
{
	t_philo		*philosopher;

	philosopher = master->philo_table[id - 1];
	while (timestamp(master) <= philosopher->time_to_die)
	{
		check_fork_status(master, id);
		if (philosopher->is_eating == FALSE && philosopher->has_forks == 2)
		{
			pthread_mutex_lock(&master->mutex_message);
			print_message(master, 2, id);
			philosopher->is_thinking = FALSE;
			philosopher->is_eating = TRUE;
			pthread_mutex_unlock(&master->mutex_message);
			philosopher->time_to_die = timestamp(master) + master->time_to_die;
			wait_action(master, id, master->time_to_eat);
			clean_the_forks(master, id);
			if (check_simulation_status(master))
				break ;
			philosopher->number_of_times_has_eaten++;
			return ;
		}
		else if (philosopher->is_thinking == FALSE)
			philo_think(master, id);
	}
	kill_philosopher(master, id);
}

void	go_to_table(t_master *master, int id)
{
	if (!check_simulation_status(master))
	{
		master->philo_table[id - 1]->is_sleeping = FALSE;
		philo_eat(master, id);
	}
}
