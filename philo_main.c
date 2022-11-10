/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daejlee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 19:41:33 by daejlee           #+#    #+#             */
/*   Updated: 2022/10/25 19:41:35 by daejlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philosophers.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// each fork must be protected by mutex.
/* time vals are mixed up. -> need revising.
let's use just one struct time and let manager handles it.
-> this causes a lot of inconvenience.
let's use one struct time per one philo
-> this causes difference of time criteria between philos.
let's use struct time from the MAIN THREAD ONLY.
*/

static int	get_threads(t_philo_args args, t_philo_profile *profile)
{
	int	i;

	i = 0;
	while (i < args.philo_num)
	{
		if (pthread_create(&(profile[i].thr), NULL,
			routine, &(profile[i])) < 0)
			return (1);
		i++;
	}
	return (0);
}

/*
다수의 철학자(200 근접)일 때 사망 후 데드락 걸리는 현상 -> 2. 해결.
다수의 철학자 일 때 너무 빨리 죽어버림 -> 식사 순서를 정해야 한다 -> 1.
*/

int	main(int argc, char **argv)
{
	t_philo_args	args;
	t_philo_manager	manager;

	gettimeofday(&manager.time, NULL);
	if ((argc != 5 && argc != 6) || prep_args(&args, argv))
	{
		printf("invalid args\n");
		return (0);
	}
	if (init_mtx(&manager, args))
		return (1);
	if (init_manager(&manager, args))
		return (1);
	init_profile(&manager, &args);
	if (get_threads(args, manager.profile))
	{
		recover_thr_free_mem(&manager, args);
		return (1);
	}
	recover_thr_free_mem(&manager, args);
	return (0);
}
