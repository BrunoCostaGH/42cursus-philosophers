/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 15:19:26 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/03/30 19:48:24 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

void	clean_the_forks(t_master *master, int id)
{
	t_philo		*philosopher;
	t_fork		*fork_1;
	t_fork		*fork_2;
	
	philosopher = master->philo_table[id - 1];
	fork_1 = master->forks_table[id - 1];
	if (id < master->number_of_philosophers)
		fork_2 = master->forks_table[id];
	else
		fork_2 = master->forks_table[0];
	pthread_mutex_lock(&fork_1->mutex_fork);
	pthread_mutex_lock(&fork_2->mutex_fork);
	fork_1->is_being_used = FALSE;
	fork_2->is_being_used = FALSE;
	philosopher->has_forks = FALSE;
	pthread_mutex_unlock(&fork_2->mutex_fork);
	pthread_mutex_unlock(&fork_1->mutex_fork);
}

void	check_fork_status(t_master *master, int id)
{
	t_philo		*philosopher;
	t_fork		*fork_1;
	t_fork		*fork_2;
	
	philosopher = master->philo_table[id - 1];
	fork_1 = master->forks_table[id - 1];
	if (id < master->number_of_philosophers)
		fork_2 = master->forks_table[id];
	else
		fork_2 = master->forks_table[0];
	pthread_mutex_lock(&fork_1->mutex_fork);
	pthread_mutex_lock(&fork_2->mutex_fork);
	if (fork_1->is_being_used == FALSE)
	{
		if (fork_2->is_being_used == FALSE)
		{
			pthread_mutex_lock(&master->mutex_message);
			print_message(master, 1, id);
			print_message(master, 1, id);
			fork_1->is_being_used = TRUE;
			fork_2->is_being_used = TRUE;
			philosopher->has_forks = 2;
			pthread_mutex_unlock(&master->mutex_message);
		}
	}
	pthread_mutex_unlock(&fork_2->mutex_fork);
	pthread_mutex_unlock(&fork_1->mutex_fork);
}
