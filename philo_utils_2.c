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
#include <stdio.h> //printf for debugging

static void	init_profile_seg(t_philo_profile *p, t_philo_manager *manager, t_philo_args *args, int i)
{
	p->idx = i + 1;
	p->die_time = args->die_time;
	p->eat_time = args->eat_time;
	p->sleep_time = args->sleep_time;
	p->time_init_val = manager->time.tv_sec;
	p->time_adr = &manager->time;
	if (args->eat_max != -1)
		p->eat_max_adr = &(args->eat_max);
	else
		p->eat_max_adr = NULL;
	p->t_flag_adr = &manager->t_flag;
	p->m_fork_slot[0] = manager->m_fork[i];
	if (!i && args->philo_num == 1)
		p->m_fork_slot[1] = NULL;
	else if (i == args->philo_num - 1)
		p->m_fork_slot[1] = manager->m_fork[0];
	else
		p->m_fork_slot[1] = manager->m_fork[i + 1];
	p->m_time_adr = &manager->m_time;
	p->m_eat_max_adr = &manager->m_eat_max;
	p->m_t_flag_adr = &manager->m_t_flag;
}

void	init_profile(t_philo_manager *manager, t_philo_args *args)
{
	int	i;

	i = 0;
	while (i < args->philo_num)
	{
		init_profile_seg(&manager->profile[i], manager, args, i);
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
	pthread_mutex_destroy(&manager->m_time);
	pthread_mutex_destroy(&manager->m_t_flag);
	if (args.eat_max != -1)
		pthread_mutex_destroy(&manager->m_eat_max);
	i = 0;
	while (i < args.philo_num)
	{
		free(manager->m_fork[i++]);
	}
	free (manager->m_fork);
	free (manager->profile);
}

int	init_manager(t_philo_manager *manager, t_philo_args args)
{
	manager->t_flag = 0;
	manager->eat_max = args.eat_max;
	manager->profile = (t_philo_profile *)malloc(sizeof(t_philo_profile)
			* args.philo_num);
	if (!manager->profile)
		return (1);
	return (0);
}
