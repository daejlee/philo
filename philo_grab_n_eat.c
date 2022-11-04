/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_grab_n_eat.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daejlee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 19:41:38 by daejlee           #+#    #+#             */
/*   Updated: 2022/10/25 19:41:39 by daejlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philosophers.h"
#include <stdio.h>
#include <unistd.h>

int	is_termination(t_philo_profile *p_info, struct timeval *time)
{
	__uint64_t	temp;

	pthread_mutex_lock(p_info->m_t_flag_adr);
	if (*(p_info->t_flag_adr))
	{
		pthread_mutex_unlock(p_info->m_t_flag_adr);
		return (0);
	}

	if (p_info->eat_max_adr)
	{
		pthread_mutex_lock(p_info->m_eat_max_adr);
		if (!*(p_info->eat_max_adr))
		{
			pthread_mutex_lock(p_info->m_t_flag_adr);
			*(p_info->t_flag_adr) = 1;
			pthread_mutex_unlock(p_info->m_t_flag_adr);
			pthread_mutex_unlock(p_info->m_eat_max_adr);
			return (0);
		}
	}

	gettimeofday(time, NULL);
	temp = time->tv_sec / 100000 + time->tv_usec / 1000;
	if (temp >= p_info->die_time + p_info->r_eat)
	{
		pthread_mutex_lock(p_info->m_t_flag_adr);
		*(p_info->t_flag_adr) = 1;
		pthread_mutex_unlock(p_info->m_t_flag_adr);
		printf("%llu %i died\n", temp, p_info->idx);
		return (0);
	}
	return (1);
}

static int	gne_sleep(t_philo_profile *p, struct timeval *time)
{
	gettimeofday(time, NULL);
	p->r_sleep = time->tv_sec / 100000 + time->tv_usec / 1000;
	printf("%llu %i is sleeping\n", p->r_sleep, p->idx);
	if (p->eat_time + p->sleep_time >= p->die_time)
	{
		usleep((p->die_time - p->eat_time) * 1000);
		return (0);
	}
	usleep(p->sleep_time * 1000);
	if (!is_termination(p, time))
		return (0);
	p->r_sleep = 0;
	gettimeofday(time, NULL);
	p->r_think = time->tv_sec / 100000 + time->tv_usec / 1000;
	printf("%llu %i is thinking\n", p->r_think, p->idx);
	return (0);
}

int	grab_eat_sleep(t_philo_profile *p, struct timeval *time)
{
	gettimeofday(time, NULL);
	p->r_eat = time->tv_sec / 100000 + time->tv_usec / 1000;
	p->r_sleep = 0;
	p->r_think = 0;
	if (p->eat_max_adr)
	{
		pthread_mutex_lock(p->m_eat_max_adr);
		*(p->eat_max_adr) -= 1;
		pthread_mutex_unlock(p->m_eat_max_adr);
	}
	printf("%llu %i has taken a fork.\n", p->r_eat, p->idx);
	printf("%llu %i has taken a fork.\n", p->r_eat, p->idx);
	printf("%llu %i is eating\n", p->r_eat, p->idx);
	if (p->eat_time >= p->die_time)
	{
		usleep(p->die_time * 1000);
		pthread_mutex_unlock(p->m_fork_slot[0]);
		pthread_mutex_unlock(p->m_fork_slot[1]);
		return (0);
	}
	usleep(p->eat_time * 1000);
	pthread_mutex_unlock(p->m_fork_slot[0]);
	pthread_mutex_unlock(p->m_fork_slot[1]);
	if (!is_termination(p, time))
		return (0);
	else
		return (gne_sleep(p, time));
}
