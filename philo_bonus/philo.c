/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 14:16:03 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/04/08 19:00:50 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	spectate(int sig)
{
	(void)sig;
	// Nothing here
}

void	philo_eat(t_philo *philosopher, int id)
{
	while (timestamp() < philosopher->time_to_die)
	{
		(void)id;
		if(sem_wait(philosopher->fork_sem) == 0)
		{
			if(sem_wait(philosopher->fork_sem) == 0)
			{
				print_message(1, id);
				print_message(1, id);
				philosopher->has_forks = 2;
			}
		}
		if (philosopher->has_forks == 2)
		{
			print_message(2, id);
			usleep(500 * 1000);
			sem_post(philosopher->fork_sem);
			sem_post(philosopher->fork_sem);
		}
		else if (philosopher->is_thinking == FALSE)
			print_message(4, id);
	}
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
	while (timestamp() < philosopher->time_to_die)
	{
		philo_eat(philosopher, id);
	}
	sem_unlink(master->fork_sem_name);
	exit(1);
}

int	main(int argc, char **argv)
{
	int			i;
	int			pid;
	int			supervisor;
	t_master	*master;

	i = 0;
	pid = 0;
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
				routine(master, i + 1);
				break ;
			}
			else
				master->philo_table[i]->philo_pid = pid;
			i++;
		}
	}
	if (supervisor)
	{
		set_handler(&spectate);
		waitpid(-1, NULL, 0);
	}
	sem_unlink(master->fork_sem_name);
	free_master(master);
	return (0);
}
