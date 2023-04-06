/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 14:16:03 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/04/06 18:45:51 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	spectate(int sig)
{
	(void)sig;
	// Nothing here
}

void	philo_eat(t_master *master, int id)
{
	while (timestamp() > master->philo_table[id]->time_to_die)
	{
		//if ()
	}
}

void	routine(t_master *master, int id)
{
	master->philo_table[id]->time_to_die += timestamp();
	while (timestamp() > master->philo_table[id]->time_to_die)
	{
		philo_eat(master, id);
	}
	kill(0, SIGTERM);
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
	if (argc == 4 || argc == 5)
	{
		master = master_init(argv);
		while (i != master->number_of_philosophers)
		{
			pid = fork();
			if (!pid)
			{
				supervisor = FALSE;
				routine(master, i);
				break ;
			}
			else
				master->philo_table[i]->philo_pid = pid;
			i++;
		}
		free_master(master);
	}
	if (supervisor)
	{
		set_handler(&spectate);
		waitpid(-1, NULL, 0);
	}
	return (0);
}
