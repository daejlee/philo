#include "philosophers_bonus.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int	kill_all(pid_t *pid_arr, int philo_num)
{
	int	i;
	
	i = 0;
	while (i < philo_num)
	{
		kill(pid_arr[i++], SIGKILL);
	}
	return (0);
}

int	is_termination(t_philo_profile *profile, struct timeval *time, t_philo_manager *manager)
{
	__uint64_t	temp;

	if (sem_wait(profile->t_sem)) //다른 프로세스가 이미 종료되었거나 식사 수를 채움.
		exit (0);
	else
		sem_post(profile->t_sem);
	gettimeofday(time, NULL);
	temp = time->tv_sec / 100000 + time->tv_usec / 1000;
	if (temp >= profile->die_time + profile->r_eat)
	{
		sem_wait(profile->t_sem);
		printf("%llu %i died\n", temp, profile->idx);
		kill_all(manager->pid_arr, manager->philo_num);
		exit (0);
	}
	return (1);
}

static int	gne_sleep(t_philo_profile *p, struct timeval *time, t_philo_manager *manager)
{
	sem_post(manager->f_sem);
	sem_post(manager->f_sem);
	gettimeofday(time, NULL);
	p->r_sleep = time->tv_sec / 100000 + time->tv_usec / 1000;
	printf("%llu %i is sleeping\n", p->r_sleep, p->idx);
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
	printf("%llu %i is thinking\n", p->r_think, p->idx);
	return (0);
}

int	grab_eat_sleep(t_philo_profile *p, struct timeval *time, t_philo_manager *manager)
{
	gettimeofday(time, NULL);
	p->r_eat = time->tv_sec / 100000 + time->tv_usec / 1000;
	p->r_sleep = 0;
	p->r_think = 0;
	if (manager->m_sem)
	{
		if (sem_wait(manager->m_sem)) //식사 수를 모두 채웠을 때
			exit (0);
	}
	printf("%llu %i has taken a fork.\n", p->r_eat, p->idx);
	printf("%llu %i has taken a fork.\n", p->r_eat, p->idx);
	printf("%llu %i is eating\n", p->r_eat, p->idx);
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
