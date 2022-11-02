/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils_2_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daejlee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 13:33:52 by daejlee           #+#    #+#             */
/*   Updated: 2022/11/02 13:33:53 by daejlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philosophers_bonus.h"
#include <fcntl.h>

#ifndef SEM_FORK
# define SEM_FORK "/mysem_fork"
#endif

#ifndef SEM_MUST_EAT
# define SEM_MUST_EAT "/mysem_musteat"
#endif

#ifndef SEM_TERMINATE
# define SEM_TERMINATE "/mysem_terminate"
#endif

int	free_mem(t_philo_manager *manager)
{
	sem_close(manager->f_sem);
	sem_close(manager->m_sem);
	sem_close(manager->t_sem);
	free(manager->pid_arr);
	return (1);
}

void	init_profile(t_philo_profile *profile, t_philo_args args, sem_t *t_sem)
{
	profile->idx = 0;
	profile->r_eat = 0;
	profile->r_sleep = 0;
	profile->r_think = 0;
	profile->die_time = args.die_time;
	profile->eat_time = args.eat_time;
	profile->sleep_time = args.sleep_time;
	profile->t_sem = t_sem;
}

static int	get_sem(t_philo_manager *manager, t_philo_args args)
{
	manager->f_sem = sem_open(SEM_FORK, O_CREAT, 0644, args.philo_num);
	if (manager->f_sem == SEM_FAILED)
		return (1);
	manager->t_sem = sem_open(SEM_TERMINATE, O_CREAT, 0644, 1);
	if (manager->t_sem == SEM_FAILED)
	{
		sem_close(manager->t_sem);
		return (1);
	}
	if (args.must_eat_times == -1)
		manager->m_sem = NULL;
	else
	{
		manager->m_sem = sem_open(SEM_MUST_EAT, O_CREAT, 0644, 0);
		if (!manager->m_sem)
		{
			sem_close(manager->f_sem);
			sem_close(manager->t_sem);
			return (1);
		}
	}
	return (0);
}

int	init_manager(t_philo_manager *manager, t_philo_args args)
{
	sem_unlink(SEM_FORK);
	sem_unlink(SEM_TERMINATE);
	sem_unlink(SEM_MUST_EAT);
	manager->philo_num = args.philo_num;
	manager->must_eat_times = args.must_eat_times;
	if (get_sem(manager, args))
		return (1);
	manager->pid_arr = (pid_t *)malloc(sizeof(pid_t) * args.philo_num);
	if (!manager->pid_arr)
	{
		sem_close(manager->f_sem);
		sem_close(manager->t_sem);
		if (manager->m_sem)
			sem_close(manager->m_sem);
		return (1);
	}
	return (0);
}
