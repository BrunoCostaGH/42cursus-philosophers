/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 21:18:23 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/03/16 17:28:04 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
/*
Your(s) program(s) should take the following arguments:

number_of_philosophers time_to_die time_to_eat time_to_sleep
[number_of_times_each_philosopher_must_eat]

◦ number_of_philosophers: The number of philosophers and also the number
of forks.

◦ time_to_die (in milliseconds): If a philosopher didn’t start eating 
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

/*
	gettimeofday gets time in seconds and microseconds.
		Converts the seconds into microseconds for same unit calculation.
		Subtracts initial time to current time for current time stamp.
*/

void	*routine(void *arg)
{
	static int	i;
	t_master	*master;

	master = (t_master *)arg;
	pthread_mutex_lock(&master->mutex_routine);
	printf("Ï-m locked!!");
	if (!i)
		i = 0;
	pthread_mutex_unlock(&master->mutex_routine);
	printf("Ï-m unlocked!!");
	return (0);
}

void	thinking(void)
{
	return ;
}

void	eating(void)
{
	return ;
}

void	sleeping(void)
{
	return ;
}

int	main(int argc, char **argv)
{
	int				i;
	t_master		*master;

	if (argc >= 4)
	{
		i = 0;
		master = master_init(argv);
		pthread_mutex_init(&master->mutex_routine, NULL);
		while (i != master->number_of_philosophers)
		{
			if (pthread_create(&master->thread[i], NULL, &routine, &master))
				return (-1);
			i++;
		}
		while (--i)
		{
			if (pthread_join(master->thread[i], NULL))
				return (-1);
		}
		pthread_mutex_destroy(&master->mutex_routine);
		free_master(master);
	}
	return (0);
}
