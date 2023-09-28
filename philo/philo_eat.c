/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_eat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 15:12:05 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/09/26 18:27:50 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	philo_eat(t_master *master, t_philo *philosopher, unsigned int id)
{
	unsigned int	m_timestamp;

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
			m_timestamp = timestamp(master);
			philosopher->time_to_die = m_timestamp + master->time_to_die;
			wait_action(master, id, master->time_to_eat, m_timestamp);
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

void	go_to_table(t_master *master, unsigned int id)
{
	t_philo		*philosopher;

	philosopher = master->philo_table[id - 1];
	if (!check_simulation_status(master))
	{
		master->philo_table[id - 1]->is_sleeping = FALSE;
		philo_eat(master, philosopher, id);
	}
}
