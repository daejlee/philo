#include <pthread.h>
#include "philosophers.h"

void	*routine(void *i)
{
	int idx = (int)*i;
	while (ith_PHILO_NOT_DEAD)
		eat_think_sleep();
}

int	main(int argc, char **argv)
{
	t_philo_main p;
	pthread_mutex_t	mutex;

	if (INVALID_ARG)
		return (ERR);
	int i = 0;
	int mtx_idx = pthread_mutex_init(&mutex, NULL);
	if (mtx_idx < 0)
		return (1);
	while (i < P_NUM)
	{
		pthread_create(p.pthread[i++], NULL, routine, (void *)&i);
		if (PTHREAD_FAIL)
			return (1);
	}
	if (PHILO_DIES)
	{
		i = 0;
		while (i < P_NUM)
			pthread_join(p.pthread[i++], NULL);
	}
	pthread_mutex_destroy(&mutex);
	return (0);
}