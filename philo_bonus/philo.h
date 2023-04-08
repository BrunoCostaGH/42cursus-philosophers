/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 14:18:16 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/04/08 18:18:41 by bsilva-c         ###   ########.fr       */
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

typedef struct s_fork
{
	int	is_being_used;
}	t_fork;

typedef struct s_philo
{
	int		is_alive;
	int		is_eating;
	int		is_sleeping;
	int		is_thinking;
	int		has_forks;
	int		time_to_die;
	int		number_of_times_has_eaten;
	int		philo_pid;
	sem_t	*fork_sem;
}	t_philo;

typedef struct s_master
{
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_times_each_philosopher_must_eat;
	char			*fork_sem_name;
	sem_t			*fork_sem;
	struct s_philo	**philo_table;
}	t_master;

void    set_handler(void *fun);
void	free_master(t_master *master);
void	print_message(int message_id, int id);

int			timestamp(void);
int			ft_atoi(const char *nptr);

t_master	*master_init(char **argv);

#endif
