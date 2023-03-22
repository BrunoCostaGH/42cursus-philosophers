/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_n_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 15:23:18 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/03/22 14:14:54 by bsilva-c         ###   ########.fr       */
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
	i = 0;
	while (i != master->number_of_philosophers)
	{
		free(master->forks_table[i]);
		pthread_mutex_destroy(&master->forks_table[i++]->mutex_fork);
	}
	free(master->forks_table);
	pthread_mutex_destroy(&master->mutex_routine);
	pthread_mutex_destroy(&master->mutex_status);
	pthread_mutex_destroy(&master->mutex_time);
	free(master);
}

static void	forks_table_init(t_master *master)
{
	int		i;
	int		number_of_forks;

	i = 0;
	number_of_forks = master->number_of_philosophers;
	master->forks_table = malloc(number_of_forks * sizeof(t_fork *));
	if (!master->forks_table)
		return ;
	while (i != number_of_forks)
	{
		master->forks_table[i] = malloc(sizeof(t_fork));
		master->forks_table[i]->is_being_used = FALSE;
		pthread_mutex_init(&master->forks_table[i]->mutex_fork, NULL);
		i++;
	}
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
		master->philo_table[i]->has_forks = FALSE;
		i++;
	}
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
	master->number_of_philosophers = ft_atoi(argv[1]);
	master->time_to_die = ft_atoi(argv[2]);
	master->time_to_eat = ft_atoi(argv[3]);
	master->time_to_sleep = ft_atoi(argv[4]);
	master->philo_id_temp = 0;
	if (argv[5])
		master->number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
	else
		master->number_of_times_each_philosopher_must_eat = -1;
	philo_table_init(master);
	forks_table_init(master);
	pthread_mutex_init(&master->mutex_routine, NULL);
	pthread_mutex_init(&master->mutex_status, NULL);
	pthread_mutex_init(&master->mutex_time, NULL);
	return (master);
}
