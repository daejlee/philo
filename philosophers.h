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

// 사용하는 공유자원	-> int t_flag, int eat_max
// 사용하는 뮤텍스		-> m_t_flag, m_eat_max, fork_arr

// 헤더 변수들 정리..
typedef struct s_philo_profile
{
	int				idx;
	struct timeval	r_eat;
	struct timeval	r_sleep;
	struct timeval	r_think;
	int				die_time;
	int				eat_time;
	int				sleep_time;
	__int64_t		time_init_val;
	struct timeval	*time_adr;
	pthread_mutex_t	*m_time_adr;
	int				*eat_max_adr;
	pthread_mutex_t	*m_eat_max_adr;
	int				*t_flag_adr;
	pthread_mutex_t	*m_t_flag_adr;
	pthread_mutex_t	*m_fork_slot[2];
	pthread_t		thr;
}	t_philo_profile;

typedef struct s_philo_manager
{
	t_philo_profile	*profile;
	int				t_flag;
	pthread_mutex_t	m_t_flag;
	int				eat_max;
	pthread_mutex_t	m_eat_max;
	struct timeval	time;
	pthread_mutex_t	m_time;
	pthread_mutex_t	**m_fork;
}	t_philo_manager;

typedef struct s_philo_args
{
	int				philo_num;
	int				die_time;
	int				eat_time;
	int				sleep_time;
	int				eat_max;
}	t_philo_args;

int		ft_atoi(const char *nptr);
int		prep_args(t_philo_args *p, char **argv);
int		is_termination(t_philo_profile *p_info);
int		grab_eat_sleep(t_philo_profile *p, struct timeval *time);
void	init_profile(t_philo_manager *manager, t_philo_args *args);
int		init_manager(t_philo_manager *manager, t_philo_args args);
void	recover_thr_free_mem(t_philo_manager *manager, t_philo_args args);
int		init_mtx(t_philo_manager *manager, t_philo_args args);

#endif
