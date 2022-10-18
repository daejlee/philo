#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
#include <pthread.h>

typedef struct s_philo_time_info
{
	int	eat;
	int	sleep;
	int	think;
}	t_philo_time;

typedef struct s_philo_main_info
{
	int				philo_num;
	int				die_time;
	int				eat_time;
	int				sleep_time;
	int				must_eat_times;
	pthread_t		*pthread;
	pthread_mutex_t	mutex;
	t_philo_time	*recent_logs;
}	t_philo_main;

int	ft_atoi(const char *nptr);
int	prep_args(t_philo_main *p, char **argv);

#endif
