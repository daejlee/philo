#include "philosophers.h"
#include <stdio.h>
#include <unistd.h>

int	is_termination(t_philo_profile *p_info, struct timeval *time)
{
	__uint64_t	temp;

	pthread_mutex_lock(p_info->mtx);
	if (*(p_info->t_flag_adr))
	{
		pthread_mutex_unlock(p_info->mtx);
		return (0);
	}
	if (!*(p_info->eat_max))
	{
		*(p_info->t_flag_adr) = 1;
		pthread_mutex_unlock(p_info->mtx);
		return (0);
	}
	gettimeofday(time, NULL);
	temp = time->tv_sec / 100000 + time->tv_usec / 1000;
	if (temp >= p_info->die_time + p_info->r_eat)
	{
		*(p_info->t_flag_adr) = 1;
		printf("%lu %i died\n", temp, p_info->idx);
		pthread_mutex_unlock(p_info->mtx);
		return (0);
	}
	return (1);
}

static int	gne_sleep(t_philo_profile *p, struct timeval *time)
{
	*(p->l_fork) = 1;
	*(p->r_fork) = 1;
	gettimeofday(time, NULL);
	p->r_sleep = time->tv_sec / 100000 + time->tv_usec / 1000;
	printf("%lu %i is sleeping\n", p->r_sleep, p->idx);
	pthread_mutex_unlock(p->mtx);
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
	printf("%lu %i is thinking\n", p->r_think, p->idx);
	pthread_mutex_unlock(p->mtx);
	return (0);
}

int	grab_eat_sleep(t_philo_profile *p, struct timeval *time)
{
	*(p->l_fork) = 0;
	*(p->r_fork) = 0;
	gettimeofday(time, NULL);
	p->r_eat = time->tv_sec / 100000 + time->tv_usec / 1000;
	p->r_sleep = 0;
	p->r_think = 0;
	*(p->eat_max) -= 1;
	printf("%lu %i has taken a fork.\n", p->r_eat, p->idx);
	printf("%lu %i has taken a fork.\n", p->r_eat, p->idx);
	printf("%lu %i is eating\n", p->r_eat, p->idx);
	pthread_mutex_unlock(p->mtx);
	if (p->eat_time >= p->die_time)
	{
		usleep(p->die_time * 1000);
		return (0);
	}
	usleep(p->eat_time * 1000);
	if (!is_termination(p, time))
		return (0);
	else
		return (gne_sleep(p, time));
}