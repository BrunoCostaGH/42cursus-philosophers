/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 14:16:03 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/04/14 15:22:34 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void spectate(t_master *master)
{
	int i;

	i = 0;
	while (i < master->number_of_philosophers)
		kill(master->philo_table[i++]->philo_pid, SIGTERM);
}

static void	philo_sleep(t_philo *philosopher, int time_to_sleep, int id)
{
	sem_wait(philosopher->message_sem);
	print_message(3, id);
	sem_post(philosopher->message_sem);
	philosopher->is_sleeping = TRUE;
	if (timestamp() + time_to_sleep > philosopher->time_to_die)
	{
		usleep(philosopher->time_to_die * 1000);
		sem_wait(philosopher->death_sem);
		kill_philosopher(philosopher, id);
	}
	else
		usleep(time_to_sleep * 1000);
	philosopher->is_sleeping = FALSE;
	philosopher->is_full = FALSE;
}

static void	philo_think(t_philo *philosopher, int id)
{
	sem_wait(philosopher->message_sem);
	print_message(4, id);
	sem_post(philosopher->message_sem);
	philosopher->is_thinking = TRUE;
}

static void	philo_eat(t_philo *philosopher, int time_to_eat, int id)
{
	while (timestamp() < philosopher->time_to_die)
	{
		check_fork_status(philosopher, id);
		if (philosopher->has_forks == 2)
		{
			sem_wait(philosopher->message_sem);
			print_message(2, id);
			sem_post(philosopher->message_sem);
			philosopher->is_thinking = FALSE;
			philosopher->is_eating = TRUE;
			if (timestamp() + time_to_eat > philosopher->time_to_die)
			{
				usleep(philosopher->time_to_die * 1000);
				break ;
			}
			else
				usleep(time_to_eat * 1000);
			clean_the_forks(philosopher);
			philosopher->is_eating = FALSE;
			philosopher->is_full = TRUE;
			philosopher->number_of_times_has_eaten++;
			return ;
		}
		else if (philosopher->is_thinking == FALSE)
		{
			philo_think(philosopher, id);
			check_fork_status(philosopher, id);
		}
	}
	sem_wait(philosopher->death_sem);
	kill_philosopher(philosopher, id);
}

void	routine(t_master *master, int id)
{
	t_philo	*philosopher;

	philosopher = master->philo_table[id - 1];
	philosopher->time_to_die += timestamp();
	philosopher->fork_sem = sem_open(master->fork_sem_name, 0);
	if (!philosopher->fork_sem)
	{
		printf("\e[1;41m===%d===ERROR: fork_sem failed on open\e[0m\n", id);
		exit(1);
	}
	philosopher->message_sem = sem_open(master->message_sem_name, 0);
	if (!philosopher->message_sem)
	{
		printf("\e[1;41m===%d===ERROR: fork_sem failed on open\e[0m\n", id);
		exit(1);
	}
	philosopher->master_sem = sem_open(master->master_sem_name, 0);
	if (!philosopher->master_sem)
	{
		printf("\e[1;41m===%d===ERROR: fork_sem failed on open\e[0m\n", id);
		exit(1);
	}
	philosopher->death_sem = sem_open(master->death_sem_name, 0);
	if (!philosopher->death_sem)
	{
		printf("\e[1;41m===%d===ERROR: fork_sem failed on open\e[0m\n", id);
		exit(1);
	}
	while (timestamp() < philosopher->time_to_die && philosopher->is_alive)
	{
		if (master->number_of_philosophers > 1)
		{
			philo_eat(philosopher, master->time_to_eat, id);
			philosopher->time_to_die = timestamp() + master->time_to_die;
		}
		if (master->number_of_times_each_philosopher_must_eat > 0)
		{
			if (philosopher->number_of_times_has_eaten == master->number_of_times_each_philosopher_must_eat)
				exit(0);
		}
		if (philosopher->is_alive && philosopher->is_full)
			philo_sleep(philosopher, master->time_to_sleep, id);
	}
	sem_wait(philosopher->death_sem);
	kill_philosopher(philosopher, id);
}

int	main(int argc, char **argv)
{
	int			i;
	int			pid;
	int			supervisor;
	t_master	*master;

	if ((argc == 5 || argc == 6))
	{
		i = 0;
		supervisor = TRUE;
		master = master_init(argv);
		while (i < master->number_of_philosophers)
		{
			pid = fork();
			if (!pid)
			{
				supervisor = FALSE;
				routine(master, i + 1);
				break ;
			}
			else
				master->philo_table[i]->philo_pid = pid;
			i++;
		}
		if (supervisor)
		{
			waitpid(-1, NULL, 0);
			spectate(master);
		}
		free_master(master);
	}
	return (0);
}
