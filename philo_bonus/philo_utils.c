/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 15:50:10 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/09/26 17:18:20 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
	gettimeofday gets time in seconds and microseconds.
		Converts the seconds into microseconds for same unit calculation.
		Subtracts initial time to current time for current time stamp.
*/
int	timestamp(void)
{
	static long		ml_ini;
	long			ml_cur;
	struct timeval	timeval;

	gettimeofday(&timeval, NULL);
	ml_cur = (timeval.tv_sec * 1000000 + timeval.tv_usec);
	if (!ml_ini)
		ml_ini = ml_cur;
	return ((ml_cur - ml_ini) * 0.001);
}

/*
	[1] timestamp_in_ms X has taken a fork
	[2] timestamp_in_ms X is eating
	[3] timestamp_in_ms X is sleeping
	[4] timestamp_in_ms X is thinking
	[5] timestamp_in_ms X died
*/
void	print_message(t_philo *philosopher, int message_id, int id)
{
	int	m_timestamp;

	if (access_philosopher_status(philosopher, 0))
	{
		m_timestamp = timestamp();
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

void	wait_action(t_master *master, int id, int time_to_wait, int timestamp)
{
	t_philo			*philosopher;
	unsigned int	wait_sum;

	philosopher = master->philo_table[id];
	wait_sum = timestamp + time_to_wait;
	if (wait_sum > (unsigned int)philosopher->time_to_die)
	{
		usleep((philosopher->time_to_die - timestamp) * 1000);
		kill_philosopher(master, id);
	}
	else
		usleep(time_to_wait * 1000);
}

/*
 * status: 0 check if philosopher is alive
 * status: 1 set philosopher as dead
 * */
int	access_philosopher_status(t_philo *philosopher, int status)
{
	int	result;

	sem_wait(philosopher->vigilante_sem);
	if (status == 0 && philosopher->is_alive)
		result = TRUE;
	else if (status == 0)
		result = FALSE;
	if (status == 1)
	{
		philosopher->is_alive = FALSE;
		result = FALSE;
	}
	sem_post(philosopher->vigilante_sem);
	return (result);
}

void	kill_philosopher(t_master *master, int id)
{
	int		i;
	t_philo	*philosopher;

	i = 0;
	philosopher = master->philo_table[id];
	if (access_philosopher_status(philosopher, 0))
	{
		sem_wait(philosopher->message_sem);
		print_message(philosopher, 5, id);
		access_philosopher_status(philosopher, 1);
		while (i++ < master->number_of_philosophers)
		{
			sem_post(philosopher->death_sem);
			sem_post(philosopher->master_sem);
		}
	}
}
