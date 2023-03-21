/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 21:21:25 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/03/21 19:38:21 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

# ifndef TRUE
#  define TRUE 1
# endif

# ifndef FALSE
#  define FALSE 0
# endif

typedef struct s_fork
{
	int				is_being_used;
	pthread_mutex_t	mutex_fork;
}	t_fork;

typedef struct s_philo
{
	int	is_alive;
	int	is_eating;
	int	is_sleeping;
	int	is_thinking;
	int	has_forks;
	pthread_t		thread;
}	t_philo;

typedef struct s_master
{
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_times_each_philosopher_must_eat;
	int				philo_id_temp;
	struct s_philo	**philo_table;
	struct s_fork	**forks_table;
	pthread_t	thread;
	pthread_mutex_t	mutex_routine;
	pthread_mutex_t	mutex_time;
}	t_master;

int			timestamp(t_master *master);
int			ft_atoi(const char *nptr);
int			check_simulation_status(t_master *master);

void		free_master(t_master *master);

t_master	*master_init(char **argv);

#endif
