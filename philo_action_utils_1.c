#include "philosophers.h"
#include <stdio.h>
#include <unistd.h>

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

void	get_time(t_philo_profile *p, struct timeval *time,
		struct timeval *dest, __uint64_t *time_stamp)
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
