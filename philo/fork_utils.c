/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 15:19:26 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/09/26 18:22:07 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	clean_the_forks(t_master *master, unsigned int id)
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
	pthread_mutex_lock(&fork_2->mutex_fork);
	fork_2->is_being_used = FALSE;
	pthread_mutex_unlock(&fork_2->mutex_fork);
	pthread_mutex_lock(&fork_1->mutex_fork);
	fork_1->is_being_used = FALSE;
	pthread_mutex_unlock(&fork_1->mutex_fork);
	philosopher->has_forks = FALSE;
}

static void	grab_fork(t_master *master, t_fork *fork, unsigned int id)
{
	pthread_mutex_lock(&master->mutex_message);
	print_message(master, 1, id);
	fork->is_being_used = TRUE;
	master->philo_table[id - 1]->has_forks += 1;
	pthread_mutex_unlock(&master->mutex_message);
}

void	check_fork_status(t_master *master, unsigned int id)
{
	t_fork		*fork_1;
	t_fork		*fork_2;
	t_fork		*fork_temp;

	fork_1 = master->forks_table[id - 1];
	if (id < master->number_of_philosophers)
		fork_2 = master->forks_table[id];
	else
		fork_2 = master->forks_table[0];
	if (id % 2 == 0)
	{
		fork_temp = fork_1;
		fork_1 = fork_2;
		fork_2 = fork_temp;
	}
	pthread_mutex_lock(&fork_2->mutex_fork);
	if (fork_2->is_being_used == FALSE)
		grab_fork(master, fork_2, id);
	pthread_mutex_unlock(&fork_2->mutex_fork);
	pthread_mutex_lock(&fork_1->mutex_fork);
	if (fork_1->is_being_used == FALSE && \
		master->philo_table[id - 1]->has_forks)
		grab_fork(master, fork_1, id);
	pthread_mutex_unlock(&fork_1->mutex_fork);
}
