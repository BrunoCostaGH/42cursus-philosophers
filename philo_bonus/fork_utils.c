/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 16:10:12 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/04/17 18:03:13 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	clean_the_forks(t_philo *philosopher)
{
	if (philosopher->number_of_times_must_eat > 0)
	{
		if (philosopher->number_of_times_has_eaten == \
			philosopher->number_of_times_must_eat)
			exit(0);
	}
	sem_post(philosopher->fork_sem);
	sem_post(philosopher->fork_sem);
}

void	check_fork_status(t_philo *philosopher, int id)
{
	sem_wait(philosopher->master_sem);
	if (sem_wait(philosopher->fork_sem) == 0)
	{
		if (sem_wait(philosopher->fork_sem) == 0)
		{
			sem_wait(philosopher->message_sem);
			print_message(1, id);
			print_message(1, id);
			sem_post(philosopher->message_sem);
			philosopher->has_forks = 2;
		}
	}
	sem_post(philosopher->master_sem);
}
