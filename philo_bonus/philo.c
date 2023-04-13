/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 14:16:03 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/04/13 10:20:44 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	kill_process(int sig, t_master *master, int id)
{
	static t_philo	*philosopher;

	if (!philosopher)
		philosopher = master->philo_table[id - 1];
	else
	{
		if (sig == SIGTERM)
		{
			sem_close(philosopher->message_sem);
			sem_close(philosopher->master_sem);
			sem_close(philosopher->fork_sem);
			exit(0);
		}
	}
}

void	spectate(int sig)
{
	(void)sig;
	// Nothing here
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
		kill_philosopher(philosopher, id);
	}
	else
		usleep(time_to_sleep * 1000);
	philosopher->is_sleeping = FALSE;
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
			philosopher->number_of_times_has_eaten++;
			return ;
		}
		else if (philosopher->is_thinking == FALSE)
			philo_think(philosopher, id);
	}
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
		printf("\e[1;41m===%d===ERROR: fork_sem failed on open\e[a0m\n", id);
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
	while (timestamp() < philosopher->time_to_die && philosopher->is_alive)
	{
		philo_eat(philosopher, master->time_to_eat, id);
		philosopher->time_to_die = timestamp() + master->time_to_die;
		if (philosopher->number_of_times_has_eaten == master->number_of_times_each_philosopher_must_eat)
			break ;
		if (philosopher->is_alive)
			philo_sleep(philosopher, master->time_to_sleep, id);
	}
	kill_process(SIGTERM, master, id);
}

int	main(int argc, char **argv)
{
	int			i;
	int			pid;
	int			supervisor;
	t_master	*master;

	i = 0;
	supervisor = TRUE;
	master = master_init(argv);
	if ((argc == 4 || argc == 5) && master)
	{
		while (i != master->number_of_philosophers)
		{
			pid = fork();
			if (!pid)
			{
				supervisor = FALSE;
				set_handler(&kill_process);
				kill_process(0, master, i + 1);
				routine(master, i + 1);
				break ;
			}
			else
				master->philo_table[i]->philo_pid = pid;
			i++;
		}
	}
	if ((argc == 4 || argc == 5) && supervisor)
	{
		set_handler(&spectate);
		waitpid(-1, NULL, 0);
	}
	free_master(master);
	return (0);
}
