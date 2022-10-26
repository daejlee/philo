#include "philosophers_bonus.h"
#include <stdlib.h>
#include <semaphore.h>


int	recover_thr_free_mem(t_philo_manager *manager, t_philo_args args)
{
	int	i;

	i = 0;
	while (i < args.philo_num)
	{
		pthread_join(manager->profile[i].thr, NULL);
		i++;
	}
	pthread_mutex_destroy(&(manager->mutex));
	free (manager->fork);
	free (manager->profile);
	return (1);
}

void	init_profile(t_philo_profile *profile, t_philo_args args)
{
	profile->r_eat = 0;
	profile->r_sleep = 0;
	profile->r_think = 0;
	profile->die_time = args.die_time;
	profile->eat_time = args.eat_time;
	profile->sleep_time = args.sleep_time;
}

int	init_manager(t_philo_manager *manager, t_philo_args args)
{
	manager->f_sem = sem_open(SEM_FORK, O_CREAT, 0644, args.philo_num);
	if (manager->f_sem == SEM_FAILED)
		return (1);
	manager->pid_arr = (pid_t *)malloc(sizeof(pid_t) * args.philo_num);
	if (!manager->pid_arr)
	{
		sem_close(manager->f_sem);
		return (1);
	}
	if (manager->m_sem == -1)
	{
		manager->m_sem = NULL;
		manager->m_sem_flag = 0;
	}
	else
	{
		manager->m_sem = sem_open(SEM_MUST_EAT, O_CREAT, 0644, args.philo_num);
		manager->m_sem_flag = 1;
	}
	return (0);
}
