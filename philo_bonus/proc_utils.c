/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proc_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 18:05:36 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/04/17 19:57:34 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	spectate(t_master *master)
{
	int	i;

	i = 0;
	while (i < master->number_of_philosophers)
		kill(master->philo_table[i++]->philo_pid, SIGTERM);
}

void	proc_init(t_master *master, int *supervisor)
{
	int		i;
	pid_t	pid;

	i = 0;
	while (i < master->number_of_philosophers)
	{
		pid = fork();
		if (!pid)
		{
			*supervisor = FALSE;
			routine(master, i + 1);
			break ;
		}
		else
			master->philo_table[i]->philo_pid = pid;
		i++;
	}
}
