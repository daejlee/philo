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

// each fork must be protected by mutex.
/* time vals are mixed up. -> need revising.
let's use just one struct time and let manager handles it.
-> this causes a lot of inconvenience.
let's use one struct time per one philo
-> this causes difference of time criteria between philos.
let's use struct time from the MAIN THREAD ONLY.
*/
/*
다수의 철학자 일 때 너무 빨리 죽어버림 -> 식사 순서를 정해야 한다. -> 짝수의 경우는 괜찮음.
1, 3 / 2, 4 / 5 의 순서로 3번 먹어야 함 -> 구현 해야 함.
	./philo 5 800 200 300일 때
	A	eat[0~200]		sleep[200~500]		think[500~600]		eat[600~800]		sleep[800~1100]
	B	think[0~200]	eat[200~400]		sleep[400~700]		think[700~800]		eat[800~1000]
	C	think[0~200]	think[200~400]		eat[400~600]		sleep[600~900]		think[900~1000]
	./philo 5 800 300 200일 때
	A	eat[0~300]		sleep[300~500]		think[500~900]		eat[600~800]		sleep[800~1100]
	B	think[0~300]	eat[300~600]		sleep[600~800]		think[700~800]		eat[800~1000]
	C	think[0~300]	think[300~600]		eat[600~900]		sleep[600~900]		think[900~1000]
	eat_time * 3 - (eat_time + sleep_time)
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

int	main(int argc, char **argv)
{
	t_philo_args	args;
	t_philo_manager	manager;

	if ((argc != 5 && argc != 6) || prep_args(&args, argv))
	{
		printf("invalid args\n");
		return (0);
	}
	if (init_mtx(&manager, args))
		return (1);
	if (init_manager(&manager, args))
		return (1);
	gettimeofday(&manager.time, NULL);
	init_profile(&manager, &args);
	if (get_threads(args, manager.profile))
	{
		recover_thr_free_mem(&manager, args);
		return (1);
	}
	recover_thr_free_mem(&manager, args);
	return (0);
}
