/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 14:03:40 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/03/16 17:32:48 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static t_philo	*philo_init(void)
{
	t_philo	*philo;

	philo = malloc(sizeof(t_philo));
	philo->is_alive = TRUE;
	philo->is_eating = FALSE;
	philo->is_sleeping = FALSE;
	philo->is_thinking = FALSE;
	return (philo);
}

int	timestamp(void)
{
	int						ml_cur;
	int						ml_ini;
	struct timeval			cur_tv;
	static struct timeval	ini_tv;

	if (!ini_tv.tv_sec)
		gettimeofday(&ini_tv, NULL);
	gettimeofday(&cur_tv, NULL);
	ml_cur = (cur_tv.tv_sec * 1000000 + cur_tv.tv_usec);
	ml_ini = (ini_tv.tv_sec * 1000000 + ini_tv.tv_usec);
	return ((ml_cur - ml_ini) * 0.001);
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
	int			i;
	t_master	*master;
	t_master	*m;

	i = 0;
	master = malloc(sizeof(t_philo));
	master->number_of_philosophers = ft_atoi(argv[1]);
	master->time_to_die = ft_atoi(argv[2]);
	master->time_to_eat = ft_atoi(argv[3]);
	master->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		master->number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
	else
		master->number_of_times_each_philosopher_must_eat = -1;
	master->thread = malloc(master->number_of_philosophers * sizeof(pthread_t));
	if (!master->thread)
		return (0);
	m = master;
	m->philo_table = malloc(master->number_of_philosophers * sizeof(t_philo *));
	master->philo_table = m->philo_table;
	if (!master->philo_table)
		return (0);
	while (i != master->number_of_philosophers)
		master->philo_table[i++] = philo_init();
	return (master);
}

void	free_master(t_master *master)
{
	int	i;

	i = 0;
	free(master->thread);
	while (i != master->number_of_philosophers)
		free(master->philo_table[i++]);
	free(master);
}
