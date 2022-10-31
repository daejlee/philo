#include "philosophers_bonus.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h> //waitpid

/*
모든 포크는 탁상 중앙에 위치한다.
메모리에는 아무런 상태가 없지만 사용 가능한 포크의 수는 세마포어로 나타낸다.
각 철학자는 프로세스가 되어야 한다. 하지만 메인 프로세스는 철학자가 되선 안된다.
*/

void	*routine(t_philo_manager *manager, t_philo_profile *profile, int philo_num)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	profile->r_eat = time.tv_sec / 100000 + time.tv_usec / 1000;
	while (is_termination(profile, &time, manager))
	{
		if (philo_num == 1) //1명일 때.
		{
			usleep(profile->die_time * 1000);
			gettimeofday(&time, NULL);
			printf("%lu 1 died\n", time.tv_sec / 100000 + time.tv_usec / 1000);
			exit(0);
		}
		else if (!sem_wait(manager->f_sem))
		{
			if (!sem_wait(manager->f_sem))
				grab_eat_sleep(profile, &time, manager);
			else
				sem_post(manager->f_sem);
		}
	}
}

int	get_philos(t_philo_manager *manager, t_philo_args args, t_philo_profile *profile)
{
	int				child_count;
	pid_t			pid;

	child_count = args.philo_num;
	while (child_count)
	{
		profile->idx++;
		pid = fork();
		if (pid == -1)
			return (1);
		else if (!pid) //자식
			routine(manager, profile, args.philo_num);
		child_count--;
		manager->pid_arr[child_count] = pid;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_philo_args	args;
	t_philo_manager	manager;
	t_philo_profile	profile;
	int				child_cnt;

	if ((argc != 5 && argc != 6) || prep_args(&args, argv))
	{
		printf("invalid args\n");
		return (0);
	}
	if (init_manager(&manager, args))
		return (1);
	init_profile(&profile, args, manager.t_sem);
	if (get_philos(&manager, args, &profile))
		return (free_mem(&manager));
	child_cnt = args.philo_num;
	waitpid(-1, NULL, 0);
	free_mem(&manager);
	printf ("no?\n");
	system("leaks philo");
	return (0);
}