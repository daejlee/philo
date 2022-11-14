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

static void	init_profile_seg_b(t_philo_profile *p, t_philo_manager *manager,
		t_philo_args *args, int i)
{
	if (!i && args->philo_num == 1)
	{
		p->fork_stat[1] = NULL;
		p->m_fork_slot[1] = NULL;
	}
	else if (i == args->philo_num - 1)
	{
		p->fork_stat[1] = &manager->fork_stat[0];
		p->m_fork_slot[1] = manager->m_fork[0];
	}
	else
	{
		p->fork_stat[1] = &manager->fork_stat[i + 1];
		p->m_fork_slot[1] = manager->m_fork[i + 1];
	}
	p->m_time_adr = &manager->m_time;
	p->m_must_eat_flag = &manager->m_must_eat_flags;
	p->m_fork_stat = &manager->m_fork_stat;
	p->m_t_flag_adr = &manager->m_t_flag;
	p->manager_adr = manager;
}

static void	init_profile_seg_a(t_philo_profile *p, t_philo_manager *manager,
		t_philo_args *args, int i)
{
	p->idx = i + 1;
	p->die_time = args->die_time;
	p->eat_time = args->eat_time;
	p->sleep_time = args->sleep_time;
	p->eat_cnt = 0;
	p->must_eat = args->must_eat;
	p->time_init_val = manager->time.tv_sec * 1000
		+ manager->time.tv_usec / 1000;
	p->time_adr = &manager->time;
	if (args->must_eat != -1)
		p->must_eat_flag = &(manager->must_eat_flags[i]);
	else
		p->must_eat_flag = NULL;
	p->t_flag_adr = &manager->t_flag;
	p->fork_stat[0] = &manager->fork_stat[i];
	p->m_fork_slot[0] = manager->m_fork[i];
}

void	init_profile(t_philo_manager *manager, t_philo_args *args)
{
	int	i;

	i = 0;
	while (i < args->philo_num)
	{
		init_profile_seg_a(&manager->profile[i], manager, args, i);
		init_profile_seg_b(&manager->profile[i], manager, args, i);
		i++;
	}
}

static int	get_slots(t_philo_manager *manager, t_philo_args args)
{
	manager->must_eat_flags = (int *)malloc(sizeof(int) * args.philo_num);
	if (!manager->must_eat_flags)
		return (1);
	manager->profile = (t_philo_profile *)malloc(sizeof(t_philo_profile)
			* args.philo_num);
	if (!manager->profile)
	{
		free(manager->must_eat_flags);
		return (1);
	}
	manager->fork_stat = (int *)malloc(sizeof(int) * args.philo_num);
	if (!manager->fork_stat)
	{
		free(manager->profile);
		free(manager->must_eat_flags);
		return (1);
	}
	return (0);
}

int	init_manager(t_philo_manager *manager, t_philo_args args)
{
	int	i;

	manager->philo_num = args.philo_num;
	manager->t_flag = 0;
	if (get_slots(manager, args))
	{
		i = args.philo_num;
		while (i--)
		{
			pthread_mutex_destroy(manager->m_fork[i]);
			free(manager->m_fork[i]);
		}
		free(manager->m_fork);
		return (purge_mtx(manager, args));
	}
	i = 0;
	while (i < args.philo_num)
		manager->fork_stat[i++] = 1;
	i = 0;
	while (i < args.philo_num)
		manager->must_eat_flags[i++] = 0;
	return (0);
}
