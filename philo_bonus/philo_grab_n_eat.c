#include "philosophers.h"
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>

int	is_termination(t_philo_profile *profile, struct timeval *time)
{
	__uint64_t	temp;

	if (sem_wait)
	gettimeofday(time, NULL);
	temp = time->tv_sec / 100000 + time->tv_usec / 1000;
	if (temp >= profile->die_time + profile->r_eat)
	{
		printf("%lu %i died\n", temp, profile->idx);
		exit (0);
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

int	grab_eat_sleep(t_philo_profile *p, struct timeval *time, sem_t *m_sem)
{
	gettimeofday(time, NULL);
	p->r_eat = time->tv_sec / 100000 + time->tv_usec / 1000;
	p->r_sleep = 0;
	p->r_think = 0;
	if (m_sem)
	{
		if (sem_wait(m_sem))
			exit (0);
	}
	printf("%lu %i has taken a fork.\n", p->r_eat, p->idx);
	printf("%lu %i has taken a fork.\n", p->r_eat, p->idx);
	printf("%lu %i is eating\n", p->r_eat, p->idx);
	if (p->eat_time >= p->die_time)
	{
		usleep(p->die_time * 1000);
		return (0);
	}
	usleep(p->eat_time * 1000);
	sem_post(manager->f_sem);
	sem_post(manager->f_sem);
	if (!is_termination(p, time))
		return (0);
	else
		return (gne_sleep(p, time));
}
