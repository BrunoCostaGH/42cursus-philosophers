/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 21:18:23 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/03/08 22:58:16 by bsilva-c         ###   ########.fr       */
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

int	timestamp(void)
{
	int						ml_cur;
	int						ml_ini;
	struct timeval			cur_tv;
	static struct timeval	ini_tv;

	if (!ini_tv.tv_sec)
		gettimeofday(&ini_tv, NULL);
	gettimeofday(&cur_tv, NULL);
	ml_cur = (cur_tv.tv_sec * 1000 + cur_tv.tv_usec);
	ml_ini = (ini_tv.tv_sec * 1000 + ini_tv.tv_usec);
	return ((ml_cur - ml_ini));
}

void	*thinking(void *arg)
{
	return (0);
}

void	*eating(char *time_to_die)
{
	return (0);
}

void	*sleeping(void *arg)
{
	return (0);
}

/*
	argv[1] = number_of_philosophers;
	argv[2] = time_to_die;
	argv[3] = time_to_eat;
	argv[4] = time_to_sleep;
	argv[5] = number_of_times_each_philosopher_must_eat; *optional
*/
int	main(int argc, char *argv)
{
	int				philo_amount;
	pthread_t		*th;

	if (argc >= 4 && argv[1] > 0)
	{
		timestamp();
		printf("%ld %d is thinking...\n", timestamp(), 0);
		philo_amount = 0;
		th = malloc(argv[1] * sizeof(pthread_t));
		while (philo_amount != argv[1])
		{
			if (pthread_create(&th[philo_amount], NULL, &thinking, &argv[2]))
				return (1);
			philo_amount++;
		}
		usleep(1200000);
		printf("%ld %d is thinking...\n", timestamp(), 0);
		free(th);
	}
	return (0);
}
