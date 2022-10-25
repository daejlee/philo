/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daejlee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 19:42:09 by daejlee           #+#    #+#             */
/*   Updated: 2022/10/25 19:42:10 by daejlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <pthread.h>
# include <sys/time.h>

/*
1. 각 철학자들의 프로파일 자식 구조체(t_philo_profile)와
2. 그들을 한데에 묶어서 관리할 부모 구조체(t_philo_manager)가 필요하다.
모든 스레드의 생성마다 각각에 맞는 t_philo_profile 구조체를 전해준다.
스레드는 주어진 루틴 함수를 조건이 만족될 때 까지 무기한 실행한다.
조건이 만족되면 메인 스레드는 다른 스레드들을 모두 중지하고 회수한다.
*/

typedef struct s_philo_profile
{
	int				idx;
	__uint64_t		r_eat;
	__uint64_t		r_sleep;
	__uint64_t		r_think;
	int				die_time;
	int				eat_time;
	int				sleep_time;
	int				*eat_max;
	int				*t_flag_adr;
	int				*l_fork;
	int				*r_fork;
	pthread_mutex_t	*mtx;
	pthread_t		thr;
}	t_philo_profile;

typedef struct s_philo_args
{
	int				philo_num;
	int				die_time;
	int				eat_time;
	int				sleep_time;
	int				must_eat_times;
}	t_philo_args;

typedef struct s_philo_manager
{
	t_philo_profile	*profile;
	pthread_mutex_t	mutex;
	int				*fork;
	int				t_flag;
}	t_philo_manager;

int		ft_atoi(const char *nptr);
int		prep_args(t_philo_args *p, char **argv);
int		is_termination(t_philo_profile *p_info, struct timeval *time);
int		grab_eat_sleep(t_philo_profile *p, struct timeval *time);
void	init_profile(t_philo_manager *manager, t_philo_args *args);
int		init_manager(t_philo_manager *manager, t_philo_args args);
int		recover_thr_free_mem(t_philo_manager *manager, t_philo_args args);

#endif
