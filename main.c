/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
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

void	*routine(void *philo_info)
{
	t_philo_profile	*p_info;
	struct timeval	time;

	p_info = (t_philo_profile *)philo_info;
	gettimeofday(&time, NULL);
	p_info->r_eat = time.tv_sec / 100000 + time.tv_usec / 1000;
	while (is_termination(p_info, &time))
	{
		if (!(p_info->r_fork))
		{
			usleep(p_info->die_time * 1000);
			gettimeofday(&time, NULL);
			printf("%lu 1 died\n", time.tv_sec / 100000 + time.tv_usec / 1000);
			pthread_mutex_unlock(p_info->mtx);
			break ;
		}
		else if (*(p_info->l_fork) && *(p_info->r_fork))
			grab_eat_sleep(p_info, &time);
		else
			pthread_mutex_unlock(p_info->mtx);
	}
	return (0);
}

int	get_thread_mutex(t_philo_args p, t_philo_profile *profile)
{
	int	i;
	int	thr_id;

	i = 0;
	while (i < p.philo_num)
	{
		thr_id = pthread_create(&(profile[i].thr),
				NULL, routine, &(profile[i]));
		if (thr_id < 0)
			return (1);
		i++;
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
	mtx_id = pthread_mutex_init(&manager.mutex, NULL);
	if (mtx_id < 0)
		return (1);
	if (init_manager(&manager, args))
		return (1);
	init_profile(&manager, &args);
	if (get_thread_mutex(args, manager.profile))
		return (recover_thr_free_mem(&manager, args));
	recover_thr_free_mem(&manager, args);
	return (0);
}
