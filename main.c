#include "philosophers.h"
#include <unistd.h>
#include <pthread.h>

typedef struct s_phio
{
	int				philo_num;
	int				die_time;
	int				eat_time;
	int				sleep_time;
	int				must_eat_times;
	pthread_t		*pthread;
	pthread_mutex_t	*mutex;
}	t_philo;



int	get_thread_mutex(t_philo *p)
{
	int	philo_count;

	philo_count = p->philo_num;
	while (philo_count--)
	{
		pthread_create(p->pthread[philo_count], NULL, p_function, something);
		pthread_mutex_init(p->mutex[philo_count], NULL);
	}
}

int	main(int argc, char **argv)
{
	t_philo			p;

	if (argc != 5 && argc != 6)
	{
		write(1, "too many or less args\n", 22);
		return (0);
	}
	if (prep_args(&p, argv) || get_thread_mutex(&p))
		return (0);
	
}