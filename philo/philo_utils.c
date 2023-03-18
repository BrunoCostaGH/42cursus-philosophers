/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 14:03:40 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/03/18 18:21:51 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	timestamp(void)
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
	gettimeofday(&cur_tv, NULL);
	ml_cur = (cur_tv.tv_sec * 1000000 + cur_tv.tv_usec);
	return ((ml_cur - ml_ini) * 0.001);
}
