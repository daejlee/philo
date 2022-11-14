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

typedef struct s_philo_manager	t_philo_manager;

typedef struct s_philo_profile
{
	int				idx;
	struct timeval	r_eat;
	struct timeval	r_sleep;
	struct timeval	r_think;
	int				die_time;
	int				eat_time;
	int				sleep_time;
	int				eat_cnt;
	int				must_eat;
	__int64_t		time_init_val;
	struct timeval	*time_adr;
	pthread_mutex_t	*m_time_adr;
	int				*must_eat_flag;
	pthread_mutex_t	*m_must_eat_flag;
	int				*t_flag_adr;
	pthread_mutex_t	*m_t_flag_adr;
	int				*fork_stat[2];
	pthread_mutex_t	*m_fork_stat;
	pthread_mutex_t	*m_fork_slot[2];
	pthread_t		thr;
	t_philo_manager	*manager_adr;
}	t_philo_profile;

typedef struct s_philo_manager
{
	int				philo_num;
	t_philo_profile	*profile;
	struct timeval	time;
	pthread_mutex_t	m_time;
	int				t_flag;
	pthread_mutex_t	m_t_flag;
	int				*must_eat_flags;
	pthread_mutex_t	m_must_eat_flags;
	int				*fork_stat;
	pthread_mutex_t	m_fork_stat;
	pthread_mutex_t	**m_fork;
}	t_philo_manager;

typedef struct s_philo_args
{
	int				philo_num;
	int				die_time;
	int				eat_time;
	int				sleep_time;
	int				must_eat;
}	t_philo_args;

int		ft_atoi(const char *nptr);
int		prep_args(t_philo_args *p, char **argv);
int		is_termination(t_philo_profile *p_info);
int		grab_eat_sleep(t_philo_profile *p, struct timeval *time);
void	init_profile(t_philo_manager *manager, t_philo_args *args);
int		init_manager(t_philo_manager *manager, t_philo_args args);
int		purge_mtx(t_philo_manager *manager, t_philo_args args);
void	recover_thr_free_mem(t_philo_manager *manager, t_philo_args args);
int		init_mtx(t_philo_manager *manager, t_philo_args args);
void	*routine(void *philo_info);
int		is_fork_available(t_philo_profile *p);
int		unlock_fork(t_philo_profile *p);
void	get_time(t_philo_profile *p, struct timeval *time,
			struct timeval *dest, __uint64_t *time_stamp);
void	usleep_check(t_philo_profile *p, struct timeval *time, int targ_time);
int		is_flags_all_up(int *must_eat_flags, int philo_num);

#endif
