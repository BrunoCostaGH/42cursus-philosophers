/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 14:18:16 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/09/26 18:35:24 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <fcntl.h>
# include <errno.h>
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <pthread.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <semaphore.h>

# ifndef TRUE
#  define TRUE 1
# endif

# ifndef FALSE
#  define FALSE 0
# endif

typedef struct s_philo
{
	int				is_alive;
	int				is_eating;
	int				is_sleeping;
	int				is_thinking;
	int				is_full;
	int				has_forks;
	int				philo_pid;
	unsigned int	time_to_die;
	unsigned int	number_of_times_has_eaten;
	unsigned int	number_of_times_must_eat;
	sem_t			*m_fork_sem;
	sem_t			*fork_sem;
	sem_t			*message_sem;
	sem_t			*death_sem;
	sem_t			*vigilante_sem;
	sem_t			*master_sem;
	pthread_t		thread_main;
	pthread_t		thread_forks;
	pthread_t		thread_vigilante;
}	t_philo;

typedef struct s_master
{
	unsigned int	number_of_philosophers;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	unsigned int	number_of_times_each_philosopher_must_eat;
	unsigned int	philo_id;
	char			*m_fork_sem_name;
	char			*fork_sem_name;
	char			*message_sem_name;
	char			*death_sem_name;
	char			*vigilante_sem_name;
	char			*master_sem_name;
	sem_t			*m_fork_sem;
	sem_t			*fork_sem;
	sem_t			*message_sem;
	sem_t			*death_sem;
	sem_t			*vigilante_sem;
	sem_t			*master_sem;
	struct s_philo	**philo_table;
}	t_master;

unsigned int	timestamp(void);

int				is_valid(char **arr);
int				ft_atoi(const char *nptr);
int				access_philosopher_status(t_philo *philosopher, \
					unsigned int status);

void			proc_init(t_master *master);
void			free_master(t_master *master);
void			free_semaphores(t_master *master);
void			go_to_table(t_master *master, unsigned int id);
void			clean_the_forks(t_master *master, unsigned int id);
void			philo_think(t_philo *philosopher, unsigned int id);
void			kill_philosopher(t_master *master, unsigned int id);
void			check_fork_status(t_philo *philosopher, unsigned int id);
void			print_message(t_philo *philosopher, unsigned int message_id, \
					unsigned int id);
void			wait_action(t_master *master, unsigned int id, \
					unsigned int time_to_wait, unsigned int timestamp);

void			*routine(void *arg);
void			*kill_thread(void *arg);

t_master		*master_init(char **argv);

#endif
