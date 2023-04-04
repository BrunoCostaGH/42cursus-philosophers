/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 14:16:03 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/04/04 16:11:54 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	routine(t_master *master)
{
	(void)master;
	printf("Started process with PID %d\n", getpid());
}

int	main(int argc, char **argv)
{
	int			i;
	int			pid;
	t_master	*master;

	i = 0;
	if (argc == 4 || argc == 5)
	{
		pid = 0;
		master = master_init(argv);
		while (i != master->number_of_philosophers)
		{
			pid = fork();
			if (!pid)
			{
				routine(master);
				break ;
			}
			else
				master->philo_table[i]->philo_pid = pid;
			i++;
		}
		free_master(master);
	}
	printf("Ended process with PID %d\n", getpid());
	return (0);
}
