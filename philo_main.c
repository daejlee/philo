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
let's use one struct time per one philo
-> this causes difference of time criteria between philos.
let's use struct time from the MAIN THREAD ONLY.
*/

static void	*routine(void *philo_info)
{
	t_philo_profile	*p_info;
	struct timeval	*time;
	__int64_t		temp;

	p_info = (t_philo_profile *)philo_info;
	time = p_info->time_adr;
	pthread_mutex_lock(p_info->m_time_adr);
	gettimeofday(time, NULL);
	p_info->r_eat = *time;
	pthread_mutex_unlock(p_info->m_time_adr);
	while (is_termination(p_info))
	{
		if (!(p_info->m_fork_slot[1])) // 1명일 때.
		{
			usleep(p_info->die_time * 1000);
			gettimeofday(time, NULL);
			temp = time->tv_sec * 1000 + time->tv_usec / 1000;
			printf("%ld 1 died\n", temp);
			break ;
		}
		pthread_mutex_lock(p_info->m_fork_slot[0]);
		pthread_mutex_lock(p_info->m_fork_slot[1]);
		grab_eat_sleep(p_info, time);
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
			routine, &(profile[i])) < 0)
			return (1);
		i++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_philo_args	args;
	t_philo_manager	manager;

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
