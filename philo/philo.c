/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 21:18:23 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/03/20 19:00:09 by bsilva-c         ###   ########.fr       */
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

static void	philo_eat(t_master *master, int id, int time_to_die)
{
	int		time_to_eat;
	int		ini_timestamp;
	t_philo		*philosopher;
	t_fork		*fork_1;
	t_fork		*fork_2;

	time_to_eat = master->time_to_eat;
	ini_timestamp = timestamp();
	philosopher = master->philo_table[id - 1];
	fork_1 = master->forks_table[id - 1];
	if (id < master->number_of_philosophers)
		fork_2 = master->forks_table[id];
	else
		fork_2 = master->forks_table[0];
	while (timestamp() < time_to_die)
	{
		pthread_mutex_lock(&fork_1->mutex_fork);
		pthread_mutex_lock(&fork_2->mutex_fork);
		if (fork_1->is_being_used == FALSE)
		{
			if (fork_2->is_being_used == FALSE)
			{
				printf("%d %d has taken a fork\n", timestamp(), id);
				printf("%d %d has taken a fork\n", timestamp(), id);
				fork_1->is_being_used = TRUE;
				fork_2->is_being_used = TRUE;
				philosopher->has_forks = 2;
			}
		}
		pthread_mutex_unlock(&fork_2->mutex_fork);
		pthread_mutex_unlock(&fork_1->mutex_fork);
		if (philosopher->is_eating == FALSE && philosopher->has_forks == 2)
		{
			ini_timestamp = timestamp();
			philosopher->is_eating = TRUE;
			printf("%d %d is eating\n", ini_timestamp, id);
			usleep(time_to_eat * 1000);
			pthread_mutex_lock(&fork_2->mutex_fork);
			pthread_mutex_lock(&fork_1->mutex_fork);
			fork_1->is_being_used = FALSE;
			fork_2->is_being_used = FALSE;
			pthread_mutex_unlock(&fork_1->mutex_fork);
			pthread_mutex_unlock(&fork_2->mutex_fork);
			philosopher->is_eating = FALSE;
			philosopher->has_forks = FALSE;
			return ;
		}
		else if (philosopher->is_thinking == FALSE)
		{
			philosopher->is_thinking = TRUE;
			printf("%d %d is thinking\n", timestamp(), id);
		}
		if (philosopher->has_forks > 2)
			printf("\e[1;41m%d has %d forks! Has made enemys.\e[0m\n", id, philosopher->has_forks);
	}
	philosopher->is_alive = FALSE;
	return ;
}

// static void	philo_sleep(t_master *master, int id)
// {
// 	int	time_to_sleep;

// 	time_to_sleep = master->time_to_sleep;
// 	return ;
// }

void	*routine(void *arg)
{
	int			id;
	int			time_to_die;
	int			number_of_times_to_eat;
	t_master	*master;

	master = (t_master *)arg;
	number_of_times_to_eat = master->number_of_times_each_philosopher_must_eat;
	(void)number_of_times_to_eat;
	pthread_mutex_lock(&master->mutex_routine);
	master->philo_id_temp++;
	id = master->philo_id_temp;
	pthread_mutex_unlock(&master->mutex_routine);
	time_to_die = timestamp() + master->time_to_die;
	while (timestamp() < time_to_die)
	{
		if (master->number_of_philosophers > 1 && master->philo_table[id - 1]->is_alive)
		{
			philo_eat(master, id, time_to_die);
			time_to_die = timestamp() + master->time_to_die;
		}
		printf("%d %d is sleeping\n", timestamp(), id);
		usleep(master->time_to_sleep * 1000);
	}
	printf("%d %d died\n", timestamp(), id);
	return (0);
}

int	main(int argc, char **argv)
{
	int				i;
	t_master		*master;

	if (argc >= 4)
	{
		i = 0;
		master = master_init(argv);
		while (i != master->number_of_philosophers)
		{
			if (pthread_create(&master->thread[i], NULL, &routine, master))
				return (-1);
			usleep(60);
			i++;
		}
		i = 0;
		while (i != master->number_of_philosophers)
		{
			if (pthread_join(master->thread[i], NULL))
				return (-1);
			i++;
		}
		free_master(master);
	}
	return (0);
}
