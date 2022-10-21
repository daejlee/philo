#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
#include <pthread.h>

typedef struct s_philo_info
{
	int	idx;
	int	eat;
	int	sleep;
	int	think;
	int	*l_fork;
	int	*r_fork;
	pthread_mutex_t	*mtx;
}	t_philo_info;

typedef struct s_philo_args
{
	int				philo_num;
	int				die_time;
	int				eat_time;
	int				sleep_time;
	int				must_eat_times;
}	t_philo_args;

int	ft_atoi(const char *nptr);
int	prep_args(t_philo_args *p, char **argv);

#endif
