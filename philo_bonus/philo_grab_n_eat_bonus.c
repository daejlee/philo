#include "philosophers_bonus.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int	is_termination(t_philo_profile *profile, struct timeval *time, t_philo_manager *manager)
{
	__uint64_t	temp;

	sem_wait(profile->t_sem);
	sem_post(profile->t_sem);
	gettimeofday(time, NULL);
	temp = time->tv_sec / 100000 + time->tv_usec / 1000;
	if (temp >= profile->die_time + profile->r_eat)
	{
		sem_wait(profile->t_sem); //다른 프로세스가 이미 종료되었거나 식사 수를 채움.
		printf("%lu %i died\n", temp, profile->idx);
		exit (0);
	}
	manager->philo_num++;
	manager->philo_num--;
	return (1);
}

static int	gne_sleep(t_philo_profile *p, struct timeval *time, t_philo_manager *manager)
{
	sem_post(manager->f_sem);
	sem_post(manager->f_sem);
	gettimeofday(time, NULL);
	p->r_sleep = time->tv_sec / 100000 + time->tv_usec / 1000;
	printf("%lu %i is sleeping\n", p->r_sleep, p->idx);
	if (p->eat_time + p->sleep_time >= p->die_time)
	{
		usleep((p->die_time - p->eat_time) * 1000);
		return (0);
	}
	usleep(p->sleep_time * 1000);
	if (!is_termination(p, time, manager))
		return (0);
	p->r_sleep = 0;
	gettimeofday(time, NULL);
	p->r_think = time->tv_sec / 100000 + time->tv_usec / 1000;
	printf("%lu %i is thinking\n", p->r_think, p->idx);
	return (0);
}

int	grab_eat_sleep(t_philo_profile *p, struct timeval *time, t_philo_manager *manager)
{
	gettimeofday(time, NULL);
	p->r_eat = time->tv_sec / 100000 + time->tv_usec / 1000;
	p->r_sleep = 0;
	p->r_think = 0;
	printf("%lu %i has taken a fork.\n", p->r_eat, p->idx);
	printf("%lu %i has taken a fork.\n", p->r_eat, p->idx);
	printf("%lu %i is eating\n", p->r_eat, p->idx);
	if (manager->m_sem)
		sem_post(manager->m_sem);
	if (p->eat_time >= p->die_time)
	{
		usleep(p->die_time * 1000);
		return (0);
	}
	usleep(p->eat_time * 1000);
	sem_post(manager->f_sem);
	sem_post(manager->f_sem);
	if (!is_termination(p, time, manager))
		return (0);
	else
		return (gne_sleep(p, time, manager));
}
