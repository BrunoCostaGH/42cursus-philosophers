/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 14:03:40 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/05/13 11:15:09 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
	gettimeofday gets time in seconds and microseconds.
		Converts the seconds into microseconds for same unit calculation.
		Subtracts initial time to current time for current time stamp.
*/
int	timestamp(t_master *master)
{
	long					ml_cur;
	static long				ml_ini;
	struct timeval			cur_tv;
	static struct timeval	ini_tv;

	if (!ini_tv.tv_sec)
	{
		gettimeofday(&ini_tv, NULL);
		ml_ini = (ini_tv.tv_sec * 1000000 + ini_tv.tv_usec);
	}
	pthread_mutex_lock(&master->mutex_time);
	gettimeofday(&cur_tv, NULL);
	ml_cur = (cur_tv.tv_sec * 1000000 + cur_tv.tv_usec);
	pthread_mutex_unlock(&master->mutex_time);
	return ((ml_cur - ml_ini) * 0.001);
}

int	check_simulation_status(t_master *master)
{
	int	i;
	int	exit_overwrite;

	i = 0;
	exit_overwrite = 0;
	pthread_mutex_lock(&master->mutex_status);
	while (i != master->number_of_philosophers)
	{
		if (master->philo_table[i]->is_alive == FALSE)
			exit_overwrite = 1;
		if (master->philo_table[i]->number_of_times_has_eaten > 0 \
			&& master->philo_table[i]->number_of_times_has_eaten \
			== master->number_of_times_each_philosopher_must_eat)
			exit_overwrite = 1;
		i++;
	}
	if (exit_overwrite)
	{
		pthread_mutex_unlock(&master->mutex_message);
		pthread_mutex_unlock(&master->mutex_time);
		pthread_mutex_unlock(&master->mutex_routine);
	}
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
void	print_message(t_master *master, int message_id, int id)
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

void	wait_action(t_master *master, int id, int time_to_wait, int m_timestamp)
{
	t_philo		*philosopher;

	philosopher = master->philo_table[id - 1];
	if (m_timestamp + time_to_wait > philosopher->time_to_die)
	{
		usleep((philosopher->time_to_die - m_timestamp) * 1000);
		kill_philosopher(master, id);
	}
	else
		usleep(time_to_wait * 1000);
}

void	kill_philosopher(t_master *master, int id)
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
