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

/*
다수의 철학자 일 때 너무 빨리 죽어버림 -> 식사 순서를 정해야 한다. -> 짝수의 경우는 괜찮음.
1, 3 / 2, 4 / 5 의 순서로 3번 먹어야 함 -> 구현 해야 함.
	./philo 5 800 200 300일 때
	A	eat[0~200]		sleep[200~500]		think[500~600]		eat[600~800]		sleep[800~1100]
	B	think[0~200]	eat[200~400]		sleep[400~700]		think[700~800]		eat[800~1000]
	C	think[0~200]	think[200~400]		eat[400~600]		sleep[600~900]		think[900~1000]
	./philo 5 800 300 200일 때
	A	eat[0~300]		sleep[300~500]		think[500~900]		eat[600~800]		sleep[800~1100]
	B	think[0~300]	eat[300~600]		sleep[600~800]		think[700~800]		eat[800~1000]
	C	think[0~300]	think[300~600]		eat[600~900]		sleep[600~900]		think[900~1000]
	eat_time * 3 - (eat_time + sleep_time)
*/

void	get_time(t_philo_profile *p, struct timeval *time, struct timeval *dest, __uint64_t *time_stamp)
{
	pthread_mutex_lock(p->m_time_adr);
	gettimeofday(time, NULL);
	if (dest)
		*dest = *time;
	*time_stamp = time->tv_sec * 1000 + time->tv_usec / 1000 - p->time_init_val;
	pthread_mutex_unlock(p->m_time_adr);
}

void	usleep_check(t_philo_profile *p, struct timeval *time, int targ_time)
{
	__uint64_t	time_stamp;
	__uint64_t	r_time;

	if (!targ_time)
		return ;
	get_time(p, time, NULL, &r_time);
	while (1)
	{
		usleep(50);
		get_time(p, time, NULL, &time_stamp);
		if (time_stamp >= (__uint64_t)(targ_time + r_time))
			break ;
	}
}

void	*kill_single_philo(t_philo_profile *p, struct timeval *time)
{
	__int64_t		temp;

	usleep_check(p, time, p->die_time * 1000);
	gettimeofday(time, NULL);
	temp = time->tv_sec * 1000 + time->tv_usec / 1000 - p->time_init_val;
	printf("%lu 1 died\n", temp);
	return (NULL);
}

int	is_fork_available(t_philo_profile *p)
{
	pthread_mutex_lock(p->m_fork_stat);
	if (*p->fork_stat[0] && *p->fork_stat[1])
		return (0);
	return (1);
}

int	unlock_fork(t_philo_profile *p)
{
	pthread_mutex_lock(p->m_fork_stat);
	*p->fork_stat[0] = 1;
	*p->fork_stat[1] = 1;
	pthread_mutex_unlock(p->m_fork_stat);
	pthread_mutex_unlock(p->m_fork_slot[0]);
	pthread_mutex_unlock(p->m_fork_slot[1]);
	return (1);
}

int	is_flags_all_up(int *must_eat_flags, int philo_num)
{
	int	i;
	
	i = 0;
	while (i < philo_num)
	{
		if (!must_eat_flags[i])
			return (1);
		i++;
	}
	return (0);
}

int	is_termination(t_philo_profile *p)
{
	__uint64_t		time_stamp;

	pthread_mutex_lock(p->m_t_flag_adr);
	if (*(p->t_flag_adr))
	{
		pthread_mutex_unlock(p->m_t_flag_adr);
		return (0);
	}
	
	if (p->must_eat_flag)
	{
		pthread_mutex_lock(p->m_must_eat_flag);
		if (!is_flags_all_up(p->manager_adr->must_eat_flags, p->manager_adr->philo_num))
		{
			*(p->t_flag_adr) = 1;
			pthread_mutex_unlock(p->m_t_flag_adr);
			pthread_mutex_unlock(p->m_must_eat_flag);
			return (0);
		}
		pthread_mutex_unlock(p->m_must_eat_flag);
	}
	
	get_time(p, p->time_adr, NULL, &time_stamp);
	
	if (time_stamp + p->time_init_val > (__uint64_t)(p->die_time + p->r_eat.tv_sec * 1000 + p->r_eat.tv_usec / 1000))
	{
		*(p->t_flag_adr) = 1;
		printf("%lu %i died\n", time_stamp, p->idx);
		pthread_mutex_unlock(p->m_t_flag_adr);
		return (0);
	}
	return (1);
}

static int	gne_sleep(t_philo_profile *p, struct timeval *time)
{
	__uint64_t	time_stamp;
	int			think_time;

	get_time(p, time, &p->r_sleep, &time_stamp);
	if (!is_termination(p))
		return (1);
	printf("%lu %i is sleeping\n", time_stamp, p->idx);
	pthread_mutex_unlock(p->m_t_flag_adr);
	if (p->eat_time + p->sleep_time > p->die_time)
	{
		usleep_check(p, time, p->die_time - p->eat_time);
		return (1);
	}
	usleep_check(p, time, p->sleep_time);
	get_time(p, time, &p->r_think, &time_stamp);
	if (!is_termination(p))
		return (1);
	printf("%lu %i is thinking\n", time_stamp, p->idx);
	pthread_mutex_unlock(p->m_t_flag_adr);
	if (p->manager_adr->philo_num % 2)
		think_time = (p->eat_time * 2 - p->sleep_time);
	else
		think_time = 0;
	usleep_check(p, time, think_time);
	return (0);
}

int	grab_eat_sleep(t_philo_profile *p, struct timeval *time)
{
	__uint64_t		time_stamp;

	if (!is_termination(p))
		return (unlock_fork(p));
	pthread_mutex_unlock(p->m_t_flag_adr);
	get_time(p, time, &p->r_eat, &time_stamp);
	if (!is_termination(p))
		return (unlock_fork(p));
	printf("%lu %i is eating\n", time_stamp, p->idx);
	p->eat_cnt++;
	pthread_mutex_unlock(p->m_t_flag_adr);
	if (p->eat_cnt == p->must_eat)
	{
		pthread_mutex_lock(p->m_must_eat_flag);
		*p->must_eat_flag = 1;
		pthread_mutex_unlock(p->m_must_eat_flag);
	}
	if (p->eat_time >= p->die_time)
	{
		usleep_check(p, time, p->die_time);
		return (unlock_fork(p));
	}
	// usleep의 오차를 줄이자. 반복 호출하며 체크하는 함수가 필요.
	usleep_check(p, time, p->eat_time);
	unlock_fork(p);
	return (gne_sleep(p, time));
}

void	*routine(void *philo_info)
{
	t_philo_profile	*p;
	struct timeval	*time;
	__uint64_t		time_stamp;

	p = (t_philo_profile *)philo_info;
	time = p->time_adr;
	get_time(p, time, &p->r_eat, &time_stamp);

	if (!(p->m_fork_slot[1]))
		return (kill_single_philo(p, time));
	if (p->manager_adr->philo_num % 2)
	{
		if (p->idx == p->manager_adr->philo_num)
			usleep_check(p, time, p->eat_time * 2);
		else if (p->idx % 2)
			usleep_check(p, time, 1);
	}
	else if (p->idx % 2)
		usleep_check(p, time, 1);
	while (is_termination(p))
	{
		pthread_mutex_unlock(p->m_t_flag_adr);
		if (!is_fork_available(p))
		{
			*p->fork_stat[0] = 0;
			*p->fork_stat[1] = 0;
			pthread_mutex_unlock(p->m_fork_stat);
			if (!is_termination(p))
			{
				pthread_mutex_lock(p->m_fork_stat);
				*p->fork_stat[0] = 1;
				*p->fork_stat[1] = 1;
				pthread_mutex_unlock(p->m_fork_stat);
				return (0);
			}
			get_time(p, time, NULL, &time_stamp);
			pthread_mutex_lock(p->m_fork_slot[0]);
			printf("%lu %i has taken a fork.\n", time_stamp, p->idx);
			pthread_mutex_lock(p->m_fork_slot[1]);
			printf("%lu %i has taken a fork.\n", time_stamp, p->idx);
			pthread_mutex_unlock(p->m_t_flag_adr);
			if (grab_eat_sleep(p, time))
				break ;
		}
		else
			pthread_mutex_unlock(p->m_fork_stat);
	}
	return (0);
}
