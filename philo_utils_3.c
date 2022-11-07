#include "philosophers.h"
#include <stdlib.h>
#include <stdio.h> //for printf debugging.

static int	init_mtx_seg_a(t_philo_manager *manager, t_philo_args args)
{
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
	return (0);
}

static int	init_mtx_seg_b(t_philo_manager *manager, t_philo_args args, int i)
{
			printf("%i is i.\n", i);
	// if we printf the i, then no problen. but if we don't, we get error. -> wtf
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
	return (0);
}

int	init_mtx(t_philo_manager *manager, t_philo_args args)
{
	int	i;

	if (init_mtx_seg_a(manager, args))
		return (1);
	i = -1;
	while (i++ < args.philo_num)
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
			pthread_mutex_destroy(&manager->m_time);
			pthread_mutex_destroy(&manager->m_t_flag);
			if (args.eat_max != -1)
				pthread_mutex_destroy(&manager->m_eat_max);
			return (1);
		}
	}
	return (0);
}
