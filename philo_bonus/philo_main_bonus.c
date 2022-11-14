/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daejlee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 13:33:36 by daejlee           #+#    #+#             */
/*   Updated: 2022/11/02 13:33:57 by daejlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philosophers_bonus.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

static int	check_must_eat_sem(t_philo_manager *manager)
{
	int	i;

	i = manager->args.must_eat * manager->args.philo_num;
	while (i--)
		sem_wait(manager->must_eat_sem);
	return (0);
}

static int	get_pcs(t_philo_manager *manager,
	t_philo_args args, t_philo_profile *profile)
{
	int		child_count;
	pid_t	pid;

	child_count = args.philo_num;
	while (child_count)
	{
		profile->idx++;
		pid = fork();
		if (pid == -1)
			return (1);
		else if (!pid)
			routine(manager, profile);
		child_count--;
		manager->pid_arr[child_count] = pid;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_philo_args	args;
	t_philo_manager	manager;
	t_philo_profile	p;

	if ((argc != 5 && argc != 6) || prep_args(&args, argv))
	{
		printf("invalid args\n");
		return (0);
	}
	if (init_manager(&manager, args))
		return (1);
	init_profile(&p, args);
	if (get_pcs(&manager, args, &p))
		return (free_mem(&manager));
	if (manager.must_eat_sem)
		check_must_eat_sem(&manager);
	else
		waitpid(-1, NULL, 0);
	kill_all(manager.pid_arr, args.philo_num);
	free_mem(&manager);
	return (0);
}
