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
		manager->profile[i].time_adr = &manager->time;
		if (args->eat_max != -1)
			manager->profile[i].eat_max_adr = &(args->eat_max);
		else
			manager->profile[i].eat_max_adr = NULL;
		manager->profile[i].t_flag_adr = &manager->t_flag;
		manager->profile[i].m_fork_slot[0] = manager->m_fork[i];
		if (!i && args->philo_num == 1)
			manager->profile[i].m_fork_slot[1] = NULL;
		else if (i == args->philo_num - 1)
			manager->profile[i].m_fork_slot[1] = manager->m_fork[0];
		else
			manager->profile[i].m_fork_slot[1] = manager->m_fork[i + 1];
		manager->profile[i].m_time_adr = &manager->m_time;
		manager->profile[i].m_eat_max_adr = &manager->m_eat_max;
		manager->profile[i].m_t_flag_adr = &manager->m_t_flag;
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
		free(manager->m_fork[i++]);
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

int	init_mtx(t_philo_manager *manager, t_philo_args args)
{
	int	i;

	if (pthread_mutex_init(&manager->m_time, NULL))
		return (1);
	if (pthread_mutex_init(&manager->m_t_flag, NULL))
	{
		pthread_mutex_destroy(&manager->m_time);
		return (1);
	}
	if (args.eat_max != -1)
	{
		if (pthread_mutex_init(&manager->m_eat_max, NULL))
		{
			pthread_mutex_destroy(&manager->m_time);
			pthread_mutex_destroy(&manager->m_t_flag);
			return (1);
		}
	}
	manager->m_fork = (pthread_mutex_t **)malloc(sizeof (pthread_mutex_t *) * args.philo_num);
	if (!manager->m_fork)
	{
		pthread_mutex_destroy(&manager->m_time);
		pthread_mutex_destroy(&manager->m_t_flag);
		if (args.eat_max != -1)
			pthread_mutex_destroy(&manager->m_eat_max);
		return (1);
	}
	i = 0;
	while (i < args.philo_num)
	{
		manager->m_fork[i] = (pthread_mutex_t *)malloc(sizeof (pthread_mutex_t));
		if (!manager->m_fork[i])
		{
			while (i--)
				free(manager->m_fork[i]);
			free(manager->m_fork);
			pthread_mutex_destroy(&manager->m_time);
			pthread_mutex_destroy(&manager->m_t_flag);
			if (args.eat_max != -1)
				pthread_mutex_destroy(&manager->m_eat_max);
			return (1);
		}
		if (pthread_mutex_init(manager->m_fork[i], NULL))
		{
			while (i--)
			{
				pthread_mutex_destroy(manager->m_fork[i]);
				free(manager->m_fork[i]);
			}
			free(manager->m_fork);
			pthread_mutex_destroy(&manager->m_time);
			pthread_mutex_destroy(&manager->m_t_flag);
			if (args.eat_max != -1)
				pthread_mutex_destroy(&manager->m_eat_max);
			return (1);
		}
		i++;
	}
	return (0);
}
