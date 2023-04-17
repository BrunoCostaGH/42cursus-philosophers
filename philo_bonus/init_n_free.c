/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_n_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 14:27:27 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/04/17 18:30:51 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_master(t_master *master)
{
	int	i;

	i = 0;
	while (i != master->number_of_philosophers)
		free(master->philo_table[i++]);
	free(master->philo_table);
	sem_close(master->fork_sem);
	sem_close(master->message_sem);
	sem_close(master->master_sem);
	sem_close(master->death_sem);
	sem_unlink(master->fork_sem_name);
	sem_unlink(master->message_sem_name);
	sem_unlink(master->master_sem_name);
	sem_unlink(master->death_sem_name);
	free(master);
}

static void	philo_table_init(t_master *master)
{
	int		i;
	int		number_of_philosophers;

	i = 0;
	number_of_philosophers = master->number_of_philosophers;
	master->philo_table = malloc(number_of_philosophers * sizeof(t_philo *));
	if (!master->philo_table)
		return ;
	while (i != number_of_philosophers)
	{
		master->philo_table[i] = malloc(sizeof(t_philo));
		master->philo_table[i]->is_alive = TRUE;
		master->philo_table[i]->is_eating = FALSE;
		master->philo_table[i]->is_sleeping = FALSE;
		master->philo_table[i]->is_thinking = FALSE;
		master->philo_table[i]->is_full = FALSE;
		master->philo_table[i]->has_forks = FALSE;
		master->philo_table[i]->time_to_die = master->time_to_die;
		master->philo_table[i]->number_of_times_has_eaten = 0;
		master->philo_table[i]->number_of_times_must_eat = \
				master->number_of_times_each_philosopher_must_eat;
		master->philo_table[i]->philo_pid = 0;
		i++;
	}
}

static void	semaphores_init(t_master *master)
{
	master->fork_sem_name = "/fork_sem";
	sem_unlink(master->fork_sem_name);
	master->fork_sem = sem_open(master->fork_sem_name, 0100, 0600, \
								master->number_of_philosophers);
	master->message_sem_name = "/message_sem";
	sem_unlink(master->message_sem_name);
	master->message_sem = sem_open(master->message_sem_name, 0100, 0600, 1);
	master->master_sem_name = "/master_sem";
	sem_unlink(master->master_sem_name);
	master->master_sem = sem_open(master->master_sem_name, 0100, 0600, 1);
	master->death_sem_name = "/death_sem";
	sem_unlink(master->death_sem_name);
	master->death_sem = sem_open(master->death_sem_name, 0100, 0600, 1);
	if (!master->fork_sem || !master->message_sem || !master->master_sem || \
	!master->death_sem)
		printf("\e[1;41m===%d===ERROR: fork_sem failed on open\e[0m\n", 0);
}

/*
	argv[1] = number_of_philosophers;
	argv[2] = time_to_die;
	argv[3] = time_to_eat;
	argv[4] = time_to_sleep;
	argv[5] = number_of_times_each_philosopher_must_eat; *optional
*/
t_master	*master_init(char **argv)
{
	t_master	*master;

	master = malloc(sizeof(t_master));
	if (!master)
	{
		printf("\e[1;41m===%d===ERROR: master failed on malloc\e[0m\n", 0);
		return (0);
	}
	master->number_of_philosophers = ft_atoi(argv[1]);
	master->time_to_die = ft_atoi(argv[2]);
	master->time_to_eat = ft_atoi(argv[3]);
	master->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		master->number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
	else
		master->number_of_times_each_philosopher_must_eat = 0;
	semaphores_init(master);
	philo_table_init(master);
	return (master);
}
