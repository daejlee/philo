#include "philosophers.h"
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

/*
int	check_dead_philo(t_philo_main *p)
{
	struct timeval	time;
	int	i;

	i = 0;
	while (i < p->philo_num)
	{
		gettimeofday(&time, NULL);
		if ((time.tv_usec * 1000 - p->recent_logs[i].eat) >= p->die_time)
		{
			printf("%i %i died\n", time.tv_usec * 1000, i);
			return (0);
		}
		i++;
	}
	return (0);
}
*/

void	init_profile(t_philo_manager *manager, t_philo_args args)
{
	int	i;

	i = 0;
	while (i < args.philo_num)
		manager->fork[i++] = 1;
	i = 0;
	while (i < args.philo_num)
	{
		manager->profile[i].idx = i;
		manager->profile[i].r_eat = 0;
		manager->profile[i].r_sleep = 0;
		manager->profile[i].r_think = 0;
		manager->profile[i].die_time = args.die_time;
		manager->profile[i].eat_time = args.eat_time;
		manager->profile[i].sleep_time = args.sleep_time;
		manager->profile[i].eat_max = &args.must_eat_times;
		manager->profile[i].t_flag_adr = &manager->t_flag;
		manager->profile[i].l_fork = &(manager->fork[i]);
		if (!i && args.philo_num == 1)
			manager->profile[i].r_fork = NULL;
		else if (i == args.philo_num - 1)
			manager->profile[i].r_fork = &(manager->fork[0]);
		else
			manager->profile[i].r_fork = &manager->fork[i + 1];
		manager->profile[i++].mtx = &(manager->mutex);
	}
}

int	grab_n_eat(t_philo_profile *p, struct timeval *time)
{
	int	*i_1;
	int	*i_2;
	
	i_1 = p->l_fork;
	i_2 = p->r_fork;
	*i_1 = 0;
	*i_2 = 0;
	gettimeofday(time, NULL);
	p->r_eat = time->tv_usec;
	p->r_sleep = 0;
	p->r_think = 0;
	*(p->eat_max) -= 1;
	printf("%i %i has taken a fork.\n", p->r_eat / 1000, p->idx);
	printf("%i %i has taken a fork.\n", p->r_eat / 1000, p->idx);
	printf("%i %i is eating\n", p->r_eat / 1000, p->idx);
	pthread_mutex_unlock(p->mtx);

	usleep(p->eat_time * 1000); //다른 스레드가 작동해야함

	pthread_mutex_lock(p->mtx);
	if (*(p->t_flag_adr))
	{
		pthread_mutex_unlock(p->mtx);
		return (0);
	}
	*i_1 = 1;
	*i_2 = 1;
	gettimeofday(time, NULL);
	p->r_sleep = time->tv_usec;
	printf("%i %i is sleeping\n", p->r_sleep / 1000, p->idx);
	pthread_mutex_unlock(p->mtx);

	usleep(p->sleep_time * 1000); //다른 스레드가 작동해야함
	
	pthread_mutex_lock(p->mtx);
	if (*(p->t_flag_adr))
	{
		pthread_mutex_unlock(p->mtx);
		return (0);
	}
	p->r_sleep = 0;
	gettimeofday(time, NULL);
	p->r_think = time->tv_usec;
	printf("%i %i is thinking\n", p->r_think / 1000, p->idx);
	pthread_mutex_unlock(p->mtx);
	return (0);
}

int	is_termination(t_philo_profile *p_info, struct timeval *time)
{
	int	temp;

	pthread_mutex_lock(p_info->mtx);
	if (*(p_info->t_flag_adr))
	{
		pthread_mutex_unlock(p_info->mtx);
		return (0);
	}
	gettimeofday(time, NULL);
	temp = time->tv_usec;
	if (p_info->r_eat - temp >= p_info->die_time * 1000
		|| !p_info->eat_max)
	{
		*(p_info->t_flag_adr) = 1;
		printf("%i %i died\n", temp / 1000, p_info->idx);
		pthread_mutex_unlock(p_info->mtx);
		return (0);
	}
	return (1);
}

void	*routine(void *philo_info)
{
	t_philo_profile	*p_info;
	struct timeval	time;

	p_info = (t_philo_profile *)philo_info;
	while (is_termination(p_info, &time))
	{
		if (!(p_info->r_fork))
		{
			usleep(p_info->die_time * 1000);
			pthread_mutex_unlock(p_info->mtx);
		}
		else if (*(p_info->l_fork) && *(p_info->r_fork))
			grab_n_eat(p_info, &time);
		else
			pthread_mutex_unlock(p_info->mtx);
	}
	return (0);
}

// 쓰레드는 스택영역은 공유하지 않는다.
// ./philo num die eat sleep must_eat

int	get_thread_mutex(t_philo_args p, t_philo_profile *profile)
{
	int	i;
	int	thr_id;

	i = 0;
	while (i < p.philo_num)
	{
		thr_id = pthread_create(&(profile[i].thr), NULL, routine, &(profile[i]));
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
	int				status;
	int				i;

	if ((argc != 5 && argc != 6) || prep_args(&args, argv))
	{
		printf("invalid args\n");
		return (0);
	}
	mtx_id = pthread_mutex_init(&manager.mutex, NULL);
	if (mtx_id < 0)
		return (1);
	manager.t_flag = 0;
	manager.profile = (t_philo_profile *)malloc(sizeof(t_philo_profile) * args.philo_num);
	if (!manager.profile)
		return (1);
	manager.fork = (int *)malloc(sizeof(int) * args.philo_num);
	if (!manager.fork)
		return (1);
	init_profile(&manager, args);
	if (get_thread_mutex(args, manager.profile))
		return (1);
	i = 0;
	while (i < args.philo_num)
	{
		pthread_join(manager.profile[i].thr, (void *)&status);
		i++;
	}
}