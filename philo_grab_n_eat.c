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

int	is_termination(t_philo_profile *p)
{
	__int64_t		temp;
	struct timeval	time_now;
	pthread_mutex_lock(p->m_t_flag_adr);
	if (*(p->t_flag_adr))
	{
		pthread_mutex_unlock(p->m_t_flag_adr);
		return (0);
	}
	
	if (p->eat_max_adr)
	{
		pthread_mutex_lock(p->m_eat_max_adr);
		if (!*(p->eat_max_adr))
		{
			*(p->t_flag_adr) = 1;
			pthread_mutex_unlock(p->m_eat_max_adr);
			return (0);
		}
		pthread_mutex_unlock(p->m_eat_max_adr);
	}

	gettimeofday(&time_now, NULL);
	temp = (time_now.tv_sec - p->r_eat.tv_sec) * 1000 + (time_now.tv_usec - p->r_eat.tv_usec) / 1000;
	if (temp >= p->die_time)
	{
		*(p->t_flag_adr) = 1;
		temp = (time_now.tv_sec - p->time_init_val) * 1000 + time_now.tv_usec / 1000;
		printf("%lu %i died\n", temp, p->idx);
		pthread_mutex_unlock(p->m_t_flag_adr);
		return (0);
	}
	//pthread_mutex_unlock(p->m_t_flag_adr);
	return (1);
}

static int	gne_sleep(t_philo_profile *p, struct timeval *time)
{
	__int64_t	temp;

	pthread_mutex_lock(p->m_time_adr);
	gettimeofday(time, NULL);
	p->r_sleep = *time;
	temp = (p->r_sleep.tv_sec - p->time_init_val) * 1000 + p->r_sleep.tv_usec / 1000;
	pthread_mutex_unlock(p->m_time_adr);
	if (!is_termination(p))
		return (1);
	printf("%lu %i is sleeping\n", temp, p->idx);
	pthread_mutex_unlock(p->m_t_flag_adr);
	if (p->eat_time + p->sleep_time >= p->die_time)
	{
		usleep((p->die_time - p->eat_time) * 1000);
		return (1);
	}
	usleep(p->sleep_time * 1000);
	pthread_mutex_lock(p->m_time_adr);
	gettimeofday(time, NULL);
	p->r_think = *time;
	temp = (p->r_think.tv_sec - p->time_init_val) * 1000 + p->r_think.tv_usec / 1000;
	pthread_mutex_unlock(p->m_time_adr);
	if (!is_termination(p))
		return (1);
	printf("%lu %i is thinking\n", temp, p->idx);
	pthread_mutex_unlock(p->m_t_flag_adr);
	return (0);
}

int	grab_eat_sleep(t_philo_profile *p, struct timeval *time)
{
	__uint64_t		temp;
	if (!is_termination(p))
	{
		pthread_mutex_unlock(p->m_fork_slot[0]);
		pthread_mutex_unlock(p->m_fork_slot[1]);
		return (1);
	}
	pthread_mutex_unlock(p->m_t_flag_adr);
	pthread_mutex_lock(p->m_time_adr);
	gettimeofday(time, NULL);
	p->r_eat = *time;
	temp = (p->r_eat.tv_sec - p->time_init_val) * 1000 + p->r_eat.tv_usec / 1000;
	pthread_mutex_unlock(p->m_time_adr);
	if (!is_termination(p))
	{
		pthread_mutex_unlock(p->m_fork_slot[0]);
		pthread_mutex_unlock(p->m_fork_slot[1]);
		return (1);
	}
	// printf("%lu %i has taken a fork.\n", temp, p->idx);
	// printf("%lu %i has taken a fork.\n", temp, p->idx);
	printf("%lu %i is eating\n", temp, p->idx);
	if (p->eat_max_adr)
	{
		pthread_mutex_lock(p->m_eat_max_adr);
		*(p->eat_max_adr) -= 1;
		pthread_mutex_unlock(p->m_eat_max_adr);
	}
	pthread_mutex_unlock(p->m_t_flag_adr);
	if (p->eat_time >= p->die_time)
	{
		usleep(p->die_time * 1000);
		pthread_mutex_unlock(p->m_fork_slot[0]);
		pthread_mutex_unlock(p->m_fork_slot[1]);
		return (1);
	}
	usleep(p->eat_time * 1000);
	pthread_mutex_unlock(p->m_fork_slot[0]);
	pthread_mutex_unlock(p->m_fork_slot[1]);
	return (gne_sleep(p, time));
}
