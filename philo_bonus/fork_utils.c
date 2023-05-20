/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 16:10:12 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/05/14 17:05:39 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	clean_the_forks(t_master *master, int id)
{
	int		i;
	t_philo	*philosopher;

	i = 0;
	philosopher = master->philo_table[id - 1];
	if (philosopher->number_of_times_must_eat > 0)
	{
		if (philosopher->number_of_times_has_eaten == \
			philosopher->number_of_times_must_eat)
		{
			sem_wait(philosopher->message_sem);
			while (i++ < master->number_of_philosophers)
				sem_post(philosopher->death_sem);
		}
	}
	sem_wait(philosopher->vigilante_sem);
	philosopher->has_forks = 0;
	sem_post(philosopher->vigilante_sem);
	sem_post(philosopher->fork_sem);
	sem_post(philosopher->fork_sem);
}

static void	grab_the_fork(t_philo *philosopher, int id)
{
	sem_wait(philosopher->vigilante_sem);
	if (!philosopher->is_full)
	{
		sem_post(philosopher->vigilante_sem);
		sem_wait(philosopher->message_sem);
		print_message(philosopher, 1, id);
		sem_post(philosopher->message_sem);
		sem_wait(philosopher->vigilante_sem);
		philosopher->has_forks += 1;
	}
	sem_post(philosopher->vigilante_sem);
}

void	check_fork_status(t_philo *philosopher, int id)
{
	sem_wait(philosopher->m_fork_sem);
	if (sem_wait(philosopher->fork_sem) == 0)
		grab_the_fork(philosopher, id);
	if (sem_wait(philosopher->fork_sem) == 0)
		grab_the_fork(philosopher, id);
	sem_post(philosopher->m_fork_sem);
}
