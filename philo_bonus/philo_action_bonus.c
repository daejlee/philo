#include "philosophers_bonus.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

static void	*kill_single_philo(t_philo_profile *p, t_philo_manager *manager)
{
	__uint64_t	time_stamp;
	struct timeval	*time;

	time = &manager->time;
	usleep_check(manager, p->die_time);
	gettimeofday(time, NULL);
	time_stamp = time->tv_sec * 1000 + time->tv_usec / 1000 - manager->time_init_val;
	printf("%llu 1 died\n", time_stamp);
	exit(0);
}

static int	gne_sleep(t_philo_profile *p, t_philo_manager *manager)
{
	__uint64_t	time_stamp;
	int			think_time;

	sem_wait(manager->print_sem);
	get_time(manager, &p->r_sleep, &time_stamp);
	printf("%llu %i is sleeping\n", time_stamp, p->idx);
	sem_post(manager->print_sem);

	if (p->eat_time + p->sleep_time >= p->die_time)
	{
		usleep_check(manager, p->die_time - p->eat_time);
		return (1);
	}
	usleep_check(manager, p->sleep_time);

	sem_wait(manager->print_sem);
	get_time(manager, &p->r_think, &time_stamp);
	printf("%llu %i is thinking\n", time_stamp, p->idx);
	sem_post(manager->print_sem);

	if (manager->args.philo_num % 2)
		think_time = (p->eat_time * 2 - p->sleep_time);
	else
		think_time = 0;
	usleep_check(manager, think_time);
	return (0);
}

static int	grab_eat_sleep(t_philo_profile *p, t_philo_manager *manager)
{
	__uint64_t	time_stamp;

	sem_wait(manager->print_sem);
	get_time(manager, &p->r_eat, &time_stamp);
	printf("%llu %i is eating\n", time_stamp, p->idx);
	p->eat_cnt++;
	if (manager->must_eat_sem && manager->args.must_eat == p->eat_cnt)
		sem_post(manager->must_eat_sem);
	sem_post(manager->print_sem);

	if (p->eat_time >= p->die_time)
	{
		usleep_check(manager, p->die_time);
		return (0);
	}
	usleep_check(manager, p->eat_time);
	sem_post(manager->fork_sem);
	sem_post(manager->fork_sem);
	return (gne_sleep(p, manager));
}

void	*thr_routine(void *info)
{
	t_philo_profile	*p;

	p =  (t_philo_profile *)info;
	while (1)
		is_termination(p, p->manager_adr);
}

void	routine(t_philo_manager *manager, t_philo_profile *p)
{
	__uint64_t	time_stamp;
	pthread_t	id;

	if (manager->args.philo_num == 1)
		kill_single_philo(p, manager);

	if (pthread_create(&id, NULL, thr_routine, p) < 0) //thread for monitoring temi.
		exit(0);
	get_time(manager, &p->r_eat, NULL);
	if (manager->args.philo_num % 2)
	{
		if (p->idx == manager->args.philo_num)
			usleep_check(manager, p->eat_time * 2);
		else if (p->idx % 2)
			usleep_check(manager, 1);
	}
	else if (p->idx % 2)
		usleep_check(manager, 1);
	while (1)
	{
		sem_post(manager->termination_sem);
		if (!sem_wait(manager->fork_sem))
		{
			sem_wait(manager->print_sem);
			get_time(manager, NULL, &time_stamp);
			printf("%llu %i has taken a fork.\n", time_stamp, p->idx);
			sem_post(manager->print_sem);
			if (!sem_wait(manager->fork_sem))
			{
				sem_wait(manager->print_sem);
				get_time(manager, NULL, &time_stamp);
				printf("%llu %i has taken a fork.\n", time_stamp, p->idx);
				sem_post(manager->print_sem);
				grab_eat_sleep(p, manager);
			}
			else
				sem_post(manager->fork_sem);
		}
	}
	pthread_join(id, NULL);
}
