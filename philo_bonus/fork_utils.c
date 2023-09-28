/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 16:10:12 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/09/26 18:35:56 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	clean_the_forks(t_master *master, unsigned int id)
{
	t_philo	*philosopher;

	philosopher = master->philo_table[id];
	philosopher->number_of_times_has_eaten++;
	if (philosopher->number_of_times_must_eat > 0)
	{
		if (philosopher->number_of_times_has_eaten == \
			philosopher->number_of_times_must_eat)
			sem_post(philosopher->master_sem);
	}
	sem_wait(philosopher->vigilante_sem);
	philosopher->has_forks = 0;
	sem_post(philosopher->vigilante_sem);
	sem_post(philosopher->fork_sem);
	sem_post(philosopher->fork_sem);
}

static void	grab_the_fork(t_philo *philosopher, unsigned int id)
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

void	check_fork_status(t_philo *philosopher, unsigned int id)
{
	sem_wait(philosopher->m_fork_sem);
	if (sem_wait(philosopher->fork_sem) == 0)
		grab_the_fork(philosopher, id);
	if (sem_wait(philosopher->fork_sem) == 0)
		grab_the_fork(philosopher, id);
	sem_post(philosopher->m_fork_sem);
}
