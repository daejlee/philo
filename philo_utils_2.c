/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daejlee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 19:41:54 by daejlee           #+#    #+#             */
/*   Updated: 2022/10/25 19:41:54 by daejlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philosophers.h"
#include <stdlib.h>

void	init_profile(t_philo_manager *manager, t_philo_args *args)
{
	int	i;

	i = 0;
	while (i < args->philo_num)
	{
		manager->profile[i].idx = i + 1;
		manager->profile[i].r_eat = 0;
		manager->profile[i].r_sleep = 0;
		manager->profile[i].r_think = 0;
		manager->profile[i].die_time = args->die_time;
		manager->profile[i].eat_time = args->eat_time;
		manager->profile[i].sleep_time = args->sleep_time;
		manager->profile[i].eat_max = &(args->must_eat_times);
		manager->profile[i].t_flag_adr = &manager->t_flag;
		manager->profile[i].time_adr = &manager->time;
		manager->profile[i].m_fork_slot[0] = &manager->m_fork[i];
		if (!i && args->philo_num == 1)
			manager->profile[i].m_fork_slot[1] = NULL;
		else if (i == args->philo_num - 1)
			manager->profile[i].m_fork_slot[1] = &manager->m_fork[0];
		else
			manager->profile[i].m_fork_slot[1] = &manager->m_fork[i + 1];
		i++;
	}
}

void	recover_thr_free_mem(t_philo_manager *manager, t_philo_args args)
{
	int	i;

	i = 0;
	while (i < args.philo_num)
	{
		pthread_join(manager->profile[i].thr, NULL);
		i++;
	}
	pthread_mutex_destroy(&(manager->mutex));
	free (manager->fork);
	free (manager->profile);
}

int	init_manager(t_philo_manager *manager, t_philo_args args)
{
	manager->t_flag = 0;
	manager->profile = (t_philo_profile *)malloc(sizeof(t_philo_profile)
			* args.philo_num);
	if (!manager->profile)
		return (1);
	return (0);
}
