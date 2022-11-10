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

void	*kill_single_philo(t_philo_profile *p, struct timeval *time)
{
	__int64_t		temp;

	usleep(p->die_time * 1000);
	gettimeofday(time, NULL);
	temp = time->tv_sec * 1000 + time->tv_usec / 1000 - p->time_init_val;
	printf("%llu 1 died\n", temp);
	return (NULL);
}

int	is_fork_available(t_philo_profile *p)
{
	pthread_mutex_lock(p->m_fork_stat);
	// printf("%i has locked m_fork_stat in is_avail.\n", p->idx);
	// printf("%i, %i is status.\n", *p->fork_stat[0], *p->fork_stat[1]);
	if (*p->fork_stat[0] && *p->fork_stat[1])
	{
		*p->fork_stat[0] = 0;
		*p->fork_stat[1] = 0;
		return (0);
	}
	else
	{
		pthread_mutex_unlock(p->m_fork_stat);
		// printf("%i has unlocked m_fork_stat.\n", p->idx);
		return (1);
	}
}

int	unlock_fork(t_philo_profile *p)
{
	pthread_mutex_lock(p->m_fork_stat);
	// printf("%i has locked m_fork_stat.\n", p->idx);
	*p->fork_stat[0] = 1;
	*p->fork_stat[1] = 1;
	pthread_mutex_unlock(p->m_fork_stat);
	// printf("%i has unlocked m_fork_stat.\n", p->idx);
	pthread_mutex_unlock(p->m_fork_slot[0]);
	pthread_mutex_unlock(p->m_fork_slot[1]);
	return (1);
}

int	is_termination(t_philo_profile *p)
{
	__int64_t		temp;
	struct timeval	time_now;

//	printf("%i has entered the t_check\n", p->idx);
	pthread_mutex_lock(p->m_t_flag_adr);
//	printf("%i has acquired t_flag mutex.\n", p->idx);
	if (*(p->t_flag_adr))
	{
		pthread_mutex_unlock(p->m_t_flag_adr);
	//	printf("%i has dropped t_flag mutex and leaving.\n", p->idx);
		return (0);
	}
	
	if (p->eat_max_adr)
	{
		pthread_mutex_lock(p->m_eat_max_adr);
		if (!*(p->eat_max_adr))
		{
			*(p->t_flag_adr) = 1;
			pthread_mutex_unlock(p->m_t_flag_adr);
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
		temp = time_now.tv_sec * 1000 + time_now.tv_usec / 1000 - p->time_init_val;
		printf("%llu %i died\n", temp, p->idx);
		pthread_mutex_unlock(p->m_t_flag_adr);
//		printf("%i has dropped t_flag mutex\n", p->idx);
		return (0);
	}
//	printf("%i has passed the t_check\n", p->idx);
	return (1);
}

static int	gne_sleep(t_philo_profile *p, struct timeval *time)
{
	__int64_t	temp;

	pthread_mutex_lock(p->m_time_adr);
	gettimeofday(time, NULL);
	p->r_sleep = *time;
	temp = p->r_sleep.tv_sec * 1000 + p->r_sleep.tv_usec / 1000 - p->time_init_val;
	pthread_mutex_unlock(p->m_time_adr);
	if (!is_termination(p))
		return (1);
	printf("%llu %i is sleeping\n", temp, p->idx);
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
	temp = p->r_think.tv_sec * 1000 + p->r_think.tv_usec / 1000 - p->time_init_val;
	pthread_mutex_unlock(p->m_time_adr);
	if (!is_termination(p))
		return (1);
	printf("%llu %i is thinking\n", temp, p->idx);
	pthread_mutex_unlock(p->m_t_flag_adr);
	return (0);
}

int	grab_eat_sleep(t_philo_profile *p, struct timeval *time)
{
	__uint64_t		temp;
	if (!is_termination(p))
		return (unlock_fork(p));
	pthread_mutex_unlock(p->m_t_flag_adr);
	pthread_mutex_lock(p->m_time_adr);
	gettimeofday(time, NULL);
	p->r_eat = *time;
	temp = p->r_eat.tv_sec * 1000 + p->r_eat.tv_usec / 1000 - p->time_init_val;
	pthread_mutex_unlock(p->m_time_adr);

	if (!is_termination(p))
		return (unlock_fork(p));
	printf("%llu %i is eating\n", temp, p->idx);
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
		return (unlock_fork(p));
	}
	usleep(p->eat_time * 1000);
	unlock_fork(p);
	return (gne_sleep(p, time));
}

void	*routine(void *philo_info)
{
	t_philo_profile	*p;
	struct timeval	*time;
	__int64_t		temp;

	p = (t_philo_profile *)philo_info;
	time = p->time_adr;
	pthread_mutex_lock(p->m_time_adr);
	gettimeofday(time, NULL);
	p->r_eat = *time;
	pthread_mutex_unlock(p->m_time_adr);

	if (!(p->m_fork_slot[1]))
		return (kill_single_philo(p, time));

	if (p->idx % 2)
		usleep(1000);

	while (is_termination(p))
	{
		pthread_mutex_unlock(p->m_t_flag_adr);
		if (!is_fork_available(p))
		{
	//		printf("%i is trying to get fork\n", p->idx);
			pthread_mutex_lock(p->m_fork_slot[0]);
			pthread_mutex_lock(p->m_fork_slot[1]);
			pthread_mutex_unlock(p->m_fork_stat);
			// printf("%i has unlocked m_fork_stat.\n", p->idx);

			if (!is_termination(p))
			{
				unlock_fork(p);
				printf("%i returning..\n", p->idx);
				return (0);
			}
			pthread_mutex_lock(p->m_time_adr);
			gettimeofday(time, NULL);
			temp = time->tv_sec * 1000 + time->tv_usec / 1000 - p->time_init_val;
			printf("%llu %i has taken a fork.\n", temp, p->idx);
			printf("%llu %i has taken a fork.\n", temp, p->idx);
			pthread_mutex_unlock(p->m_t_flag_adr);

			pthread_mutex_unlock(p->m_time_adr);
			if (grab_eat_sleep(p, time))
			{
			//	printf("breaking..\n");
				break ;
			}
		}
//		printf("%i couldn't get the fork.\n", p->idx);
	}
	printf("%i returning..\n", p->idx);
	return (0);
}

/*
sleeping -> good
eating -> good
thinking ?
5 died
7 was eating
4 was eating
3 was thinking
2 was eating

5 has died
1 was thinking
6 was thinking
*/
