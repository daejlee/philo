#include "philosophers.h"
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct s_phio
{
	int				philo_num;
	int				die_time;
	int				eat_time;
	int				sleep_time;
	int				must_eat_times;
	pthread_t		*tid;
	pthread_mutex_t	*mutex;
}	t_philo;

int	prep_args(t_philo *p, char **argv)
{
	p->philo_num = ft_atoi(argv[1]);
	p->die_time = ft_atoi(argv[2]);
	p->eat_time = ft_atoi(argv[3]);
	p->sleep_time = ft_atoi(argv[4]);
	if (argv[5])
		p->must_eat_times = ft_atoi(argv[5]);
	else
		p->must_eat_times = 0;
	if (p->philo_num < 1 || p->die_time < 1
		|| p->eat_time < 1 || p->sleep_time < 1
		|| p->must_eat_times < 0)
		{
			write(1, "invalid arg\n", 14);
			return (1);
		}
	p->tid = (pthread_t *)malloc(sizeof (pthread_t) * p->philo_num);
	if (!p->tid)
		return (1);
	p->mutex = (pthread_mutex_t *)malloc(sizeof (pthread_mutex_t) * p->philo_num);
	if (!p->mutex)
	{
		free (p->tid);
		return (1);
	}
	return (0);
}

int	get_thread_mutex(t_philo *p)
{
	int	philo_count;

	philo_count = p->philo_num;
	while (philo_count--)
	{
		p->tid[philo_count] = pthread_create();
		p->mutex[philo_count] = pthread_mutex_init();
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