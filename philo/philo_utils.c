/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 14:03:40 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/03/22 19:40:24 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	timestamp(t_master *master)
{
	int						ml_cur;
	static int				ml_ini;
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

	i = 0;
	while (i != master->number_of_philosophers)
	{
		if (master->philo_table[i]->is_alive == FALSE)
			return (1);
		i++;
	}
	return (0);
}
