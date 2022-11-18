#include "philosophers.h"
#include <stdio.h>

static void	ed_take_fork_n_eat(t_philo_profile *p,
		struct timeval *time, __uint64_t *time_stamp)
{
	pthread_mutex_lock(p->m_fork_stat);
	*p->fork_stat[0] = 0;
	*p->fork_stat[1] = 0;
	pthread_mutex_unlock(p->m_fork_stat);
	get_time(p, time, NULL, time_stamp);
	pthread_mutex_lock(p->m_fork_slot[0]);
	printf("%llu %i has taken a fork.\n", *time_stamp, p->idx);
	pthread_mutex_lock(p->m_fork_slot[1]);
	printf("%llu %i has taken a fork.\n", *time_stamp, p->idx);
	get_time(p, time, &p->r_eat, time_stamp);
	printf("%llu %i is eating\n", *time_stamp, p->idx);
}

static int	death_while_eating(t_philo_profile *p, struct timeval *time)
{
	__uint64_t	time_stamp;

	if (p->idx % 2)
		usleep_check(p, time, p->die_time + 1);
	else
	{
		ed_take_fork_n_eat(p, time, &time_stamp);
		usleep_check(p, time, p->die_time + 10);
		get_time(p, time, &p->r_eat, &time_stamp);
	}
	is_termination(p);
	return (1);
}

static int	death_while_sleeping(t_philo_profile *p, struct timeval *time)
{
	__uint64_t	time_stamp;

	if (p->idx % 2)
	{
		usleep_check(p, time, p->eat_time);
		ed_take_fork_n_eat(p, time, &time_stamp);
		usleep_check(p, time, p->die_time - p->eat_time + 2);
	}
	else
	{
		ed_take_fork_n_eat(p, time, &time_stamp);
		usleep_check(p, time, p->eat_time);
		unlock_fork(p);
		get_time(p, time, &p->r_sleep, &time_stamp);
		printf("%llu %i is sleeping\n", time_stamp, p->idx);
		usleep_check(p, time, p->die_time - p->eat_time + 1);
	}
	is_termination(p);
	return (1);
}

// ./philo 2 200 210 200
// ./philo 2 200 190 200
int	early_death(t_philo_profile *p, struct timeval *time)
{
	if (p->eat_time > p->die_time)
		return (death_while_eating(p, time));
	if (p->eat_time + p->sleep_time > p->die_time)
		return (death_while_sleeping(p, time));
	else
		return (1);
}
