#include "philosophers.h"
#include <stdlib.h>
#include <pthread.h>

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
		return (-1);
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

static int	get_slots(t_philo *p)
{
	p->pthread = (pthread_t *)malloc(sizeof (pthread_t) * p->philo_num);
	p->mutex = (pthread_mutex_t *)malloc(sizeof (pthread_mutex_t) * p->philo_num);
	if (!p->pthread || !p->pthr_id || !p->mutex || !p->mtx_id)
	{
		if (p->pthread)
			free (p->pthread);
		if (p->mutex)
			free (p->mutex);
		return (1);
	}
	return (0);
}

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
	if (get_slots(p))
		return (1);
	return (0);
}
