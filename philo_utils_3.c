/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daejlee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 12:51:32 by daejlee           #+#    #+#             */
/*   Updated: 2022/11/16 12:51:33 by daejlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <stdlib.h>

int	purge_mtx(t_philo_manager *manager, t_philo_args args)
{
	pthread_mutex_destroy(&manager->m_time);
	pthread_mutex_destroy(&manager->m_t_flag);
	pthread_mutex_destroy(&manager->m_fork_stat);
	if (args.must_eat != -1)
		pthread_mutex_destroy(&manager->m_must_eat_flags);
	return (1);
}

static int	get_mtx(t_philo_manager *manager, t_philo_args args)
{
	if (pthread_mutex_init(&manager->m_time, NULL))
		return (1);
	if (pthread_mutex_init(&manager->m_t_flag, NULL))
	{
		pthread_mutex_destroy(&manager->m_time);
		return (1);
	}
	if (pthread_mutex_init(&manager->m_fork_stat, NULL))
	{
		pthread_mutex_destroy(&manager->m_time);
		pthread_mutex_destroy(&manager->m_t_flag);
		return (1);
	}
	if (args.must_eat != -1)
	{
		if (pthread_mutex_init(&manager->m_must_eat_flags, NULL))
		{
			pthread_mutex_destroy(&manager->m_time);
			pthread_mutex_destroy(&manager->m_t_flag);
			pthread_mutex_destroy(&manager->m_fork_stat);
			return (1);
		}
	}
	return (0);
}

static int	init_mtx_seg_b(t_philo_manager *manager, t_philo_args args, int i)
{
	manager->m_fork[i] = (pthread_mutex_t *)malloc(sizeof (pthread_mutex_t));
	if (!manager->m_fork[i])
	{
		while (i--)
			free(manager->m_fork[i]);
		free(manager->m_fork);
		return (purge_mtx(manager, args));
	}
	return (0);
}

static int	init_mtx_seg_a(t_philo_manager *manager, t_philo_args args)
{
	if (get_mtx(manager, args))
		return (1);
	manager->m_fork = (pthread_mutex_t **)malloc(sizeof (pthread_mutex_t *)
			* args.philo_num);
	if (!manager->m_fork)
		return (purge_mtx(manager, args));
	return (0);
}

int	init_mtx(t_philo_manager *manager, t_philo_args args)
{
	int	i;

	if (init_mtx_seg_a(manager, args))
		return (1);
	i = 0;
	while (i < args.philo_num)
	{
		if (init_mtx_seg_b(manager, args, i))
			return (1);
		if (pthread_mutex_init(manager->m_fork[i], NULL))
		{
			while (i--)
			{
				pthread_mutex_destroy(manager->m_fork[i]);
				free(manager->m_fork[i]);
			}
			free(manager->m_fork);
			return (purge_mtx(manager, args));
		}
		i++;
	}
	return (0);
}
