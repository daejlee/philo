/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daejlee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 19:41:33 by daejlee           #+#    #+#             */
/*   Updated: 2022/10/25 19:41:35 by daejlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philosophers.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// each fork must be protected by mutex.
/* time vals are mixed up. -> need revising.
let's use just one struct time and let manager handles it.
-> this causes a lot of inconvenience.
let's use one struct time per one philo */

static void	*routine(void *philo_info)
{
	t_philo_profile	*p_info;

	p_info = (t_philo_profile *)philo_info;
	pthread_mutex_lock(&p_info->m_time);
	gettimeofday(p_info->time_adr, NULL);
	p_info->r_eat = p_info->time_adr.tv_sec / 100000 + p_info->time_adr->tv_usec / 1000;
	while (is_termination(p_info, p_info->time_adr))
	{
		if (!(p_info->r_fork))
		{
			usleep(p_info->die_time * 1000);
			gettimeofday(p_info->time_adr, NULL);
			printf("%ld 1 died\n", time->tv_sec / 100000 + time->tv_usec / 1000);
			pthread_mutex_unlock(p_info->mtx);
			break ;
		}
		else if (*(p_info->l_fork) && *(p_info->r_fork))
			grab_eat_sleep(p_info, p_info->time_adr);
		else
			pthread_mutex_unlock(p_info->mtx);
	}
	return (0);
}

static int	get_threads(t_philo_args args, t_philo_profile *profile)
{
	int	i;

	i = 0;
	while (i < args.philo_num)
	{
		if (pthread_create(&(profile[i].thr), NULL,
			routine, &(profile[i])) < 0);
			return (1);
		i++;
	}
	return (0);
}

int	init_mtx(t_philo_manager *manager, t_philo_args args)
{
	unsigned int	i;

	manager->m_fork = (pthread_mutex_t *)(sizeof (pthread_mutex_t) * args.philo_num);
	if (!manager->m_fork)
		return (1);
	while (i < args.philo_num)
	{
		if (pthread_mutex_init(&manager->m_fork[i], NULL))
		{
			while (i--)
				pthread_mutex_destroy(&manager->m_fork[i]);
			return (1);
		}
	}
	if (pthread_mutex_init(&manager->m_time, NULL))
	{
		pthread_mutex_destroy(&manager->m_time);
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_philo_args	args;
	t_philo_manager	manager;
	int				mtx_id;

	if ((argc != 5 && argc != 6) || prep_args(&args, argv))
	{
		printf("invalid args\n");
		return (0);
	}
	if (init_mtx(&manager, args))
		return (1);
	if (init_manager(&manager, args))
		return (1);
	init_profile(&manager, &args);
	if (get_threads(args, manager.profile))
	{
		recover_thr_free_mem(&manager, args);
		return (1);
	}
	recover_thr_free_mem(&manager, args);
	return (0);
}
