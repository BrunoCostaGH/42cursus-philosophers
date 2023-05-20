/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 15:44:50 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/05/10 15:45:02 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	is_valid(char **arr)
{
	int		i;
	int		k;

	i = 1;
	while (arr[i])
	{
		k = 0;
		while (arr[i][k])
		{
			if (!ft_isdigit(arr[i][k]))
				return (FALSE);
			k++;
		}
		if (i == 1 && ft_atoi(arr[i]) == 0)
			return (FALSE);
		i++;
	}
	return (TRUE);
}
