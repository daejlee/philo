#include "philosophers.h"
#include <stdlib.h>
#include <unistd.h>

static const char	*blank_check(const char *nptr)
{
	unsigned int	i;

	i = 0;
	while ((nptr[i] >= 8 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	return (&nptr[i]);
}

static int	outlier_check(unsigned int res, int sign)
{
	if (res > 2147483648 && sign == -1)
		return (0);
	else if (res > 2147483647 && sign == 1)
		return (-2);
	else
		return (res * sign);
}

int	ft_atoi(const char *nptr)
{
	int					sign;
	unsigned int		i;
	unsigned int		res;

	nptr = blank_check(nptr);
	sign = 1;
	i = 0;
	res = 0;
	if (nptr[i] == '-')
	{
		sign *= -1;
		i++;
	}
	else if (nptr[i] == '+')
		i++;
	while (nptr[i] <= '9' && nptr[i] >= '0')
	{
		res = res * 10 + nptr[i] - '0';
		i++;
	}
	res = outlier_check(res, sign);
	return (res);
}
/*
static int	get_slots(t_philo_main *p)
{
	int				i;
	t_philo_time	*temp;

	p->pthread = (pthread_t *)malloc(sizeof (pthread_t) * p->philo_num);
	p->recent_logs = (t_philo_time *)malloc(sizeof (t_philo_time) * p->philo_num);
	p->fork = (int *)malloc(sizeof (int) * p->philo_num);
	if (!p->pthread || !p->recent_logs || !p->fork)
	{
		if (p->pthread)
			free (p->pthread);
		if (p->recent_logs)
			free (p->recent_logs);
		if (p->fork)
			free (p->fork);
		return (1);
	}
	i = 0;
	while (i < p->philo_num)
	{
		p->fork[i] = 1;
		temp = &(p->recent_logs[i++]);
		temp->eat = 0;
		temp->sleep = 0;
		temp->think = 0;
	}
	return (0);
}
*/
int	prep_args(t_philo_args *p, char **argv)
{
	p->philo_num = ft_atoi(argv[1]);
	p->die_time = ft_atoi(argv[2]);
	p->eat_time = ft_atoi(argv[3]);
	p->sleep_time = ft_atoi(argv[4]);
	if (argv[5])
		p->must_eat_times = ft_atoi(argv[5]);
	else
		p->must_eat_times = -1;
	if (p->philo_num < 1 || p->die_time < 1
		|| p->eat_time < 1 || p->sleep_time < 1
		|| p->must_eat_times < -1)
			return (1);
	return (0);
}
