#include "philosophers.h"
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

int	grab_n_eat(t_philo_main *p, int *i_1, *int i_2)
{
	struct timeval	time;
	
	gettimeofday(&time, NULL);
	p->fork[i_1] = 0;
	p->fork[i_2] = 0;
	p->recent_logs[i_1].eat = time.tv_usec * 1000;
	p->recent_logs[i_1].sleep = 0;
	p->recent_logs[i_1].think = 0;
	printf("%i %i has taken a fork %i, %i\n", i_1, p->recent_logs[i_1].eat, i_1, i_2);
	printf("%i %i is eating\n", i_1, p->recent_logs[i_1].eat);
	pthread_mutex_unlock(&(p->mutex));

	usleep(p->eat_time * 1000); //다른 스레드가 작동해야함

	pthread_mutex_lock(&(p->mutex));
	gettimeofday(&time, NULL);
	p->fork[i_1] = 1;
	p->fork[i_2] = 1;
	p->recent_logs[i_1].sleep = time.tv_usec * 1000;
	p->recent_logs[i_1].think = 0;
	printf("%i %i is sleeping\n", i_1, p->recent_logs[i_1].sleep);
	pthread_mutex_unlock(&(p->mutex));

	usleep(p->sleep_time * 1000); //다른 스레드가 작동해야함
	
	pthread_mutex_lock(&(p->mutex));
	p->recent_logs[i_1].sleep = 0;
	gettimeofday(&time, NULL);
	p->recent_logs[i_1].think = time.tv_usec * 1000;
	printf("%i %i is thinking\n", i_1, p->recent_logs[i_1].think);
	pthread_mutex_unlock(&(p->mutex));
	return (0);
}

void	*routine(void *p_info)
{
	while (IM_NOT_DEAD!!)
	{
		pthread_mutex_lock(p_info->mtx);
		if (p_info->l_fork && p_info->r_fork)
			grab_n_eat(p_info, p->l_fork, p->r_fork);
		else
			pthread_mutex_unlock(p_info->mtx);
	}
	pthread_mutex_unlock(p_info->mtx);
	return (0);
}

int	get_thread_mutex(t_philo_main *p)
{
	int	i;
	int	thr_id;
	int	mtx_id;
	pthread_t		*slots;

	i = 0;
	slots = p->pthread;
	mtx_id = pthread_mutex_init(&(p->mutex), NULL);
	if (mtx_id < 0)
		return (1);
	while (i < p->philo_num)
	{
		p->philo_info[i].idx = i;
		thr_id = pthread_create(&slots[i], NULL, (void *)routine, (void *)(p->philo_info[i]));
		if (thr_id < 0)
			return (1);
		i++;
	}
	return (0);
}

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

// 쓰레드는 스택영역은 공유하지 않는다.
// ./philo num die eat sleep must_eat

int	main(int argc, char **argv)
{
	t_philo_args	p;
	int				status;
	int				i;

	if ((argc != 5 && argc != 6) || prep_args(&p, argv))
	{
		printf("invalid args\n");
		return (0);
	}
	if ()
		return (0);
	i = 0;
	while (i < p.philo_num)
		pthread_join(p.pthread[i++], (void *)&status);
}