#include "philosophers.h"
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

void	init_trd(void)
{
	struct timeval	mytime;

	gettimeofday(&mytime, NULL);
	write(1, "philosopher is here\n", 21);
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
		thr_id = pthread_create(&slots[i++], NULL, (void *)init_trd, NULL);
		if (thr_id < 0)
			return (1);
	}
	return (0);
}

int	grab_n_eat(t_philo_main *p, int i_1, int i_2)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	pthread_mutex_lock(&(p->mutex));
	p->fork[i_1] = 0;
	p->fork[i_2] = 0;
	p->recent_logs[i_1].eat = time.tv_usec * 1000;
	p->recent_logs[i_1].sleep = 0;
	p->recent_logs[i_1].think = 0;
	printf("%i %i has taken a fork\n", i_1, p->recent_logs[i_1].eat);
	printf("%i %i is eating\n", i_1, p->recent_logs[i_1].eat);
	pthread_mutex_unlock(&(p->mutex));
	usleep(p->eat_time * 1000);
	gettimeofday(&time, NULL);
	pthread_mutex_lock(&(p->mutex));
	p->fork[i_1] = 1;
	p->fork[i_2] = 1;
	p->recent_logs[i_1].sleep = time.tv_usec * 1000;
	p->recent_logs[i_1].think = 0;
	printf("%i %i is sleeping\n", i_1, p->recent_logs[i_1].sleep);
	pthread_mutex_unlock(&(p->mutex));
	usleep(p->sleep_time * 1000);
	gettimeofday(&time, NULL);
	pthread_mutex_lock(&(p->mutex));
	p->recent_logs[i_1].sleep = 0;
	p->recent_logs[i_1].think = time.tv_usec * 1000;
	printf("%i %i is thinking\n", i_1, p->recent_logs[i_1].think);
	pthread_mutex_unlock(&(p->mutex));
	return (0);
}

int	eat_think_sleep(t_philo_main *p)
{
	int	i;

	i = 0;
	while (i < p->philo_num)
	{
		if (p->fork[i] && p->fork[i + 1])
			grab_n_eat(p, i, i + 1);
		else if (i && p->fork[i] && p->fork[i - 1])
			grab_n_eat(p, i, i - 1);
		else if (!i && p->fork[i] && p->fork[p->philo_num - 1])
			grab_n_eat(p, i, p->philo_num - 1);
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

//쓰레드는 스택영역은 공유하지 않는다.

int	main(int argc, char **argv)
{
	t_philo_main	p;
	int				status;
	int				i;

	if (argc != 5 && argc != 6)
	{
		printf("too many or less args\n");
		return (0);
	}
	if (prep_args(&p, argv) || get_thread_mutex(&p))
		return (0);
	// 스레드들이 생성됨.
	eat_think_sleep(&p);
	while (!check_dead_philo(&p)) //|| !num_fulfilled(&p))
		eat_think_sleep(&p);
	i = 0;
	while (i < p.philo_num)
		pthread_join(p.pthread[i++], (void *)&status);
}