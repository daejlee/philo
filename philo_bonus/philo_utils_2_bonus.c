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
#include <signal.h>
#include <stdlib.h>

#ifndef SEM_FORK
# define SEM_FORK "/mysem_fork"
#endif

#ifndef SEM_MUST_EAT
# define SEM_MUST_EAT "/mysem_must_eat"
#endif

#ifndef SEM_TERMINATE
# define SEM_TERMINATE "/mysem_terminate"
#endif

#ifndef SEM_TIME
# define SEM_TIME "/mysem_time"
#endif

#ifndef SEM_PRINT
# define SEM_PRINT "/mysem_print"
#endif

int	kill_all(pid_t *pid_arr, int philo_num)
{
	int	i;

	i = 0;
	while (i < philo_num)
		kill(pid_arr[i++], SIGKILL);
	return (0);
}

int	free_mem(t_philo_manager *manager)
{
	sem_close(manager->fork_sem);
	sem_close(manager->termination_sem);
	sem_close(manager->time_sem);
	sem_close(manager->print_sem);
	if (manager->must_eat_sem)
		sem_close(manager->must_eat_sem);
	free(manager->pid_arr);
	return (1);
}

void	init_profile(t_philo_profile *profile, t_philo_args args)
{
	profile->idx = 0;
	profile->die_time = args.die_time;
	profile->eat_time = args.eat_time;
	profile->sleep_time = args.sleep_time;
	profile->eat_cnt = 0;
}

static int	get_sem_seg(t_philo_manager *manager)
{
	manager->termination_sem = sem_open(SEM_TERMINATE, O_CREAT, 0644, 1);
	if (manager->termination_sem == SEM_FAILED)
	{
		sem_close(manager->fork_sem);
		return (1);
	}
	manager->time_sem = sem_open(SEM_TIME, O_CREAT, 0644, 1);
	if (manager->time_sem == SEM_FAILED)
	{
		sem_close(manager->fork_sem);
		sem_close(manager->termination_sem);
		return (1);
	}
	manager->print_sem = sem_open(SEM_PRINT, O_CREAT, 0644, 1);
	if (manager->print_sem == SEM_FAILED)
	{
		sem_close(manager->fork_sem);
		sem_close(manager->termination_sem);
		sem_close(manager->time_sem);
		return (1);
	}
	return (0);
}

static int	get_sem(t_philo_manager *manager)
{
	manager->fork_sem = sem_open(SEM_FORK, O_CREAT,
			0644, manager->args.philo_num);
	if (manager->fork_sem == SEM_FAILED)
		return (1);
	if (get_sem_seg(manager))
		return (1);
	if (manager->args.must_eat == -1)
		manager->must_eat_sem = NULL;
	else
	{
		manager->must_eat_sem = sem_open(SEM_MUST_EAT, O_CREAT, 0644, 0);
		if (!manager->must_eat_sem)
		{
			sem_close(manager->fork_sem);
			sem_close(manager->termination_sem);
			sem_close(manager->time_sem);
			sem_close(manager->print_sem);
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
	sem_unlink(SEM_TIME);
	sem_unlink(SEM_PRINT);
	manager->args = args;
	if (get_sem(manager))
		return (1);
	manager->pid_arr = (pid_t *)malloc(sizeof(pid_t) * args.philo_num);
	if (!manager->pid_arr)
	{
		sem_close(manager->fork_sem);
		sem_close(manager->termination_sem);
		sem_close(manager->time_sem);
		sem_close(manager->print_sem);
		if (manager->must_eat_sem)
			sem_close(manager->must_eat_sem);
		return (1);
	}
	gettimeofday(&manager->time, NULL);
	manager->time_init_val = manager->time.tv_sec * 1000
		+ manager->time.tv_usec / 1000;
	return (0);
}
