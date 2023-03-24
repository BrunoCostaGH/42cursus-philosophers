/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 21:21:25 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/03/24 13:22:40 by bsilva-c         ###   ########.fr       */
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
	int			is_alive;
	int			is_eating;
	int			is_sleeping;
	int			is_thinking;
	int			has_forks;
	int			number_of_times_has_eaten;
	pthread_t	thread;
}	t_philo;

typedef struct s_master
{
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_times_each_philosopher_must_eat;
	int				philo_id_temp;
	pthread_t		thread;
	struct s_philo	**philo_table;
	struct s_fork	**forks_table;
	pthread_mutex_t	mutex_routine;
	pthread_mutex_t	mutex_message;
	pthread_mutex_t	mutex_status;
	pthread_mutex_t	mutex_time;
}	t_master;

int			timestamp(t_master *master);
int			ft_atoi(const char *nptr);
int			check_simulation_status(t_master *master);

void		free_master(t_master *master);
void		philo_think(t_master *master, int id);
void		clean_the_forks(t_master *master, int id);
void		kill_philosopher(t_master *master, int id);
void		check_fork_status(t_master *master, int id);
void		philo_eat(t_master *master, int id, int time_to_die);
void		run_routine(t_master *master, int id, int time_to_die);
void		print_message(t_master *master, int message_id, int id);

t_master	*master_init(char **argv);

#endif
