/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_grab_n_eat_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daejlee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 13:33:30 by daejlee           #+#    #+#             */
/*   Updated: 2022/11/02 13:33:32 by daejlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philosophers_bonus.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void	usleep_check(t_philo_manager *manager, int targ_time)
{
	__uint64_t	time_stamp;
	__uint64_t	r_time;

	if (!targ_time)
		return ;
	get_time(manager, NULL, &r_time);
	while (1)
	{
		usleep(50);
		get_time(manager, NULL, &time_stamp);
		if (time_stamp >= (__uint64_t)(targ_time + r_time))
			break ;
	}
}

void	get_time(t_philo_manager *manager, struct timeval *dest, __uint64_t *time_stamp)
{
	struct timeval	*time;

	time = &manager->time;
	sem_wait(manager->time_sem);
	gettimeofday(time, NULL);
	if (dest)
		*dest = *time;
	if (time_stamp)
		*time_stamp = time->tv_sec * 1000 + time->tv_usec / 1000 - manager->time_init_val;
	sem_post(manager->time_sem);
}

static void	check_death(t_philo_profile *p, t_philo_manager *manager)
{
	__uint64_t		time_stamp;
	
	get_time(manager, NULL, &time_stamp);
	if (time_stamp + manager->time_init_val > (__uint64_t)(p->die_time
		+ p->r_eat.tv_sec * 1000 + p->r_eat.tv_usec / 1000))
	{
		printf("%lu %i died\n", time_stamp, p->idx);
		exit (0);
	}
}

void	is_termination(t_philo_profile *p, t_philo_manager *manager)
{
	sem_wait(manager->termination_sem);
	sem_post(manager->termination_sem);

	sem_wait(manager->termination_sem);

	check_death(p, manager);
}

