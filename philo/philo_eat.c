/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_eat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 15:12:05 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/03/30 19:25:31 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	philo_eat(t_master *master, int id)
{
	int			time_to_eat;
	t_philo		*philosopher;

	time_to_eat = master->time_to_eat;
	philosopher = master->philo_table[id - 1];
	while (timestamp(master) < philosopher->time_to_die)
	{
		check_fork_status(master, id);
		if (philosopher->is_eating == FALSE && philosopher->has_forks == 2)
		{
			pthread_mutex_lock(&master->mutex_message);
			print_message(master, 2, id);
			philosopher->is_thinking = FALSE;
			philosopher->is_eating = TRUE;
			pthread_mutex_unlock(&master->mutex_message);
			if (timestamp(master) + time_to_eat > philosopher->time_to_die)
			{
				usleep(philosopher->time_to_die * 1000);
				break ;
			}
			else
				usleep(time_to_eat * 1000);
			clean_the_forks(master, id);
			philosopher->number_of_times_has_eaten++;
			return ;
		}
		else if (philosopher->is_thinking == FALSE)
			philo_think(master, id);
	}
	kill_philosopher(master, id);
	return ;
}

void	go_to_table(t_master *master, int id)
{
	int	*time_to_die;

	time_to_die = &master->philo_table[id - 1]->time_to_die;
	if (master->number_of_philosophers > 1 \
		&& !check_simulation_status(master))
	{
		pthread_mutex_unlock(&master->mutex_status);
		master->philo_table[id - 1]->is_sleeping = FALSE;
		philo_eat(master, id);
		*time_to_die = timestamp(master) + master->time_to_die;
		pthread_mutex_lock(&master->mutex_status);
	}
	else if (master->number_of_philosophers == 1)
	{
		pthread_mutex_unlock(&master->mutex_status);
		pthread_mutex_lock(&master->mutex_message);
		print_message(master, 4, id);
		master->philo_table[id - 1]->is_thinking = TRUE;
		pthread_mutex_unlock(&master->mutex_message);
		usleep(*time_to_die * 1000);
		kill_philosopher(master, id);
		pthread_mutex_lock(&master->mutex_status);
	}
}
