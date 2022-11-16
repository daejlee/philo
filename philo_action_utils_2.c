/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_action_utils_2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daejlee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 12:51:22 by daejlee           #+#    #+#             */
/*   Updated: 2022/11/16 12:51:23 by daejlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <stdio.h>

int	is_flags_all_up(int *must_eat_flags, int philo_num)
{
	int	i;

	i = 0;
	while (i < philo_num)
	{
		if (!must_eat_flags[i])
			return (1);
		i++;
	}
	return (0);
}

static int	check_death(t_philo_profile *p)
{
	__uint64_t		time_stamp;

	get_time(p, p->time_adr, NULL, &time_stamp);
	if (time_stamp + p->time_init_val > (__uint64_t)(p->die_time
		+ p->r_eat.tv_sec * 1000 + p->r_eat.tv_usec / 1000))
	{
		*(p->t_flag_adr) = 1;
		printf("%llu %i died\n", time_stamp, p->idx);
		pthread_mutex_unlock(p->m_t_flag_adr);
		return (0);
	}
	else
		return (1);
}

int	is_termination(t_philo_profile *p)
{
	pthread_mutex_lock(p->m_t_flag_adr);
	if (*(p->t_flag_adr))
	{
		pthread_mutex_unlock(p->m_t_flag_adr);
		return (0);
	}
	if (p->must_eat_flag)
	{
		pthread_mutex_lock(p->m_must_eat_flag);
		if (!is_flags_all_up(p->manager_adr->must_eat_flags,
				p->manager_adr->philo_num))
		{
			*(p->t_flag_adr) = 1;
			pthread_mutex_unlock(p->m_t_flag_adr);
			pthread_mutex_unlock(p->m_must_eat_flag);
			return (0);
		}
		pthread_mutex_unlock(p->m_must_eat_flag);
	}
	if (!check_death(p))
		return (0);
	return (1);
}
