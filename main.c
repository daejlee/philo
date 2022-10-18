#include "philosophers.h"
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

void	*init_trd(void *mutex)
{
	struct timeval	mytime;

	pthread_mutex_lock((pthread_mutex_t *)mutex);
	gettimeofday(&mytime, NULL);
	printf("philosopher is here\n");
	printf("%ld\n", mytime.tv_usec);
	pthread_mutex_unlock((pthread_mutex_t *)mutex);
	return (NULL);
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
		thr_id = pthread_create(&slots[i++], NULL, init_trd, (void *)&(p->mutex));
		if (thr_id < 0)
			return (1);
	}
	//sleep(1);
	return (0);
}

int	main(int argc, char **argv)
{
	t_philo_main			p;
	int			i;
	int						status;

	if (argc != 5 && argc != 6)
	{
		printf("too many or less args\n");
		return (0);
	}
	if (prep_args(&p, argv) || get_thread_mutex(&p))
		return (0);
	i = 0;
	while (i < p.philo_num)
	{
		pthread_join(p.pthread[i], (void *)&status);
		i++;
	}
	while (1);
}