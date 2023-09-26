/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 14:03:40 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/09/26 18:31:31 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
	gettimeofday gets time in seconds and microseconds.
		Converts the seconds into microseconds for same unit calculation.
		Subtracts initial time to current time for current time stamp.
*/
unsigned int	timestamp(t_master *master)
{
	static long		ml_ini;
	long			ml_cur;
	struct timeval	timeval;

	pthread_mutex_lock(&master->mutex_time);
	gettimeofday(&timeval, NULL);
	ml_cur = (timeval.tv_sec * 1000000 + timeval.tv_usec);
	if (!ml_ini)
		ml_ini = ml_cur;
	pthread_mutex_unlock(&master->mutex_time);
	return ((ml_cur - ml_ini) * 0.001);
}

int	check_simulation_status(t_master *master)
{
	unsigned int	i;
	unsigned int	exit_overwrite;

	i = 0;
	exit_overwrite = 0;
	pthread_mutex_lock(&master->mutex_status);
	while (i != master->number_of_philosophers)
	{
		if (master->philo_table[i]->is_alive == FALSE)
		{
			exit_overwrite = 1;
			break ;
		}
		if (master->philo_table[i]->number_of_times_has_eaten > 0 \
			&& master->philo_table[i]->number_of_times_has_eaten \
			== master->number_of_times_each_philosopher_must_eat)
			exit_overwrite++;
		i++;
	}
	if (exit_overwrite > 1 && exit_overwrite != master->number_of_philosophers)
		exit_overwrite = 0;
	pthread_mutex_unlock(&master->mutex_status);
	return (exit_overwrite);
}

/*
	[1] timestamp_in_ms X has taken a fork
	[2] timestamp_in_ms X is eating
	[3] timestamp_in_ms X is sleeping
	[4] timestamp_in_ms X is thinking
	[5] timestamp_in_ms X died
*/
void	print_message(t_master *master, unsigned int message_id, \
						unsigned int id)
{
	int	m_timestamp;

	if (!check_simulation_status(master))
	{
		m_timestamp = timestamp(master);
		if (message_id == 1)
			printf("%d %d has taken a fork\n", m_timestamp, id);
		else if (message_id == 2)
			printf("%d %d is eating\n", m_timestamp, id);
		else if (message_id == 3)
			printf("%d %d is sleeping\n", m_timestamp, id);
		else if (message_id == 4)
			printf("%d %d is thinking\n", m_timestamp, id);
		else if (message_id == 5)
			printf("%d %d died\n", m_timestamp, id);
	}
}

void	wait_action(t_master *master, unsigned int id, \
					unsigned int time_to_wait, \
					unsigned int timestamp)
{
	t_philo			*philosopher;
	unsigned int	wait_sum;

	philosopher = master->philo_table[id - 1];
	wait_sum = timestamp + time_to_wait;
	if (wait_sum > philosopher->time_to_die)
	{
		usleep((philosopher->time_to_die - timestamp) * 1000);
		kill_philosopher(master, id);
	}
	else
		usleep(time_to_wait * 1000);
}

void	kill_philosopher(t_master *master, unsigned int id)
{
	if (!check_simulation_status(master))
	{
		pthread_mutex_lock(&master->mutex_message);
		print_message(master, 5, id);
		pthread_mutex_lock(&master->mutex_status);
		master->philo_table[id - 1]->is_alive = FALSE;
		pthread_mutex_unlock(&master->mutex_status);
		pthread_mutex_unlock(&master->mutex_message);
	}
}
