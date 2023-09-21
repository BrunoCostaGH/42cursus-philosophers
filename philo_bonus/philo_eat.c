/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_eat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 17:14:07 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/05/14 17:17:56 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	philo_eat(t_master *master, int id)
{
	int		time_to_eat;
	int		m_timestamp;
	t_philo	*philosopher;

	time_to_eat = master->time_to_eat;
	philosopher = master->philo_table[id];
	if (access_philosopher_status(philosopher, 0))
	{
		sem_wait(philosopher->message_sem);
		print_message(philosopher, 2, id);
		sem_post(philosopher->message_sem);
		philosopher->is_thinking = FALSE;
		philosopher->is_eating = TRUE;
		m_timestamp = timestamp();
		philosopher->time_to_die = m_timestamp + master->time_to_die;
		wait_action(master, id, time_to_eat, m_timestamp);
		sem_wait(philosopher->vigilante_sem);
		philosopher->is_full = TRUE;
		philosopher->is_eating = FALSE;
		sem_post(philosopher->vigilante_sem);
		clean_the_forks(master, id);
	}
}

void	go_to_table(t_master *master, int id)
{
	t_philo	*philosopher;

	philosopher = master->philo_table[id];
	while (timestamp() <= philosopher->time_to_die && \
	access_philosopher_status(philosopher, 0))
	{
		sem_wait(philosopher->vigilante_sem);
		philosopher->is_full = FALSE;
		if (philosopher->has_forks == 2)
		{
			sem_post(philosopher->vigilante_sem);
			philo_eat(master, id);
			return ;
		}
		sem_post(philosopher->vigilante_sem);
		if (!philosopher->is_thinking && \
		access_philosopher_status(philosopher, 0))
			philo_think(philosopher, id);
	}
	kill_philosopher(master, id);
}
