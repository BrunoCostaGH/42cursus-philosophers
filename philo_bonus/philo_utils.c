/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 15:50:10 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/05/09 14:24:24 by bsilva-c         ###   ########.fr       */
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
	long					ml_cur;
	static long				ml_ini;
	struct timeval			cur_tv;
	static struct timeval	ini_tv;

	if (!ini_tv.tv_sec)
	{
		gettimeofday(&ini_tv, NULL);
		ml_ini = (ini_tv.tv_sec * 1000000 + ini_tv.tv_usec);
	}
	gettimeofday(&cur_tv, NULL);
	ml_cur = (cur_tv.tv_sec * 1000000 + cur_tv.tv_usec);
	return ((ml_cur - ml_ini) / 1000);
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

	if (philosopher->is_alive)
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

void	philo_semaphores_init(t_master *master, int id)
{
	t_philo	*philosopher;

	philosopher = master->philo_table[id - 1];
	philosopher->fork_sem = sem_open(master->fork_sem_name, 0);
	philosopher->message_sem = sem_open(master->message_sem_name, 0);
	philosopher->master_sem = sem_open(master->master_sem_name, 0);
	philosopher->death_sem = sem_open(master->death_sem_name, 0);
	philosopher->philo_sem = sem_open(master->philo_sem_name, 0);
	if (!philosopher->fork_sem || !philosopher->message_sem || \
		!philosopher->master_sem || !philosopher->death_sem || \
		!philosopher->philo_sem)
	{
		printf("\e[1;41m===%d===ERROR: fork_sem failed on open\e[0m\n", id);
		free_master(master);
		exit(1);
	}
}

void	wait_action(t_master *master, int id, int time_to_wait)
{
	t_philo	*philosopher;

	philosopher = master->philo_table[id - 1];
	if (timestamp() + time_to_wait > philosopher->time_to_die)
	{
		usleep((philosopher->time_to_die - timestamp()) * 1000);
		kill_philosopher(master, id);
	}
	else
		usleep(time_to_wait * 1000);
}

void	kill_philosopher(t_master *master, int id)
{
	int		i;
	t_philo	*philosopher;

	i = 0;
	philosopher = master->philo_table[id - 1];
	if (philosopher->is_alive)
	{
		sem_wait(philosopher->message_sem);
		print_message(philosopher, 5, id);
		while (i++ < master->number_of_philosophers)
			sem_post(philosopher->death_sem);
	}
}
