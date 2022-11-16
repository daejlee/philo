/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daejlee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 13:34:09 by daejlee           #+#    #+#             */
/*   Updated: 2022/11/02 13:34:10 by daejlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PHILOSOPHERS_BONUS_H
# define PHILOSOPHERS_BONUS_H
# include <pthread.h>
# include <sys/time.h>
# include <semaphore.h>

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
	t_philo_manager	*manager_adr;
}	t_philo_profile;

typedef struct s_philo_args
{
	int	philo_num;
	int	die_time;
	int	eat_time;
	int	sleep_time;
	int	must_eat;
}	t_philo_args;

typedef struct s_philo_manager
{
	t_philo_args	args;
	pid_t			*pid_arr;
	sem_t			*fork_sem;
	sem_t			*must_eat_sem;
	sem_t			*termination_sem;
	__uint64_t		time_init_val;
	struct timeval	time;
	sem_t			*time_sem;
	sem_t			*print_sem;
}	t_philo_manager;

int		ft_atoi(const char *nptr);
int		prep_args(t_philo_args *p, char **argv);

int		kill_all(pid_t *pid_arr, int philo_num);
int		free_mem(t_philo_manager *manager);
void	init_profile(t_philo_profile *profile, t_philo_args args);
int		init_manager(t_philo_manager *manager, t_philo_args args);

void	*kill_single_philo(t_philo_profile *p, t_philo_manager *manager);
void	routine(t_philo_manager *manager, t_philo_profile *p);
void	usleep_check(t_philo_manager *manager, int targ_time);
void	get_time(t_philo_manager *manager, struct timeval *dest,
			__uint64_t *time_stamp);
void	is_termination(t_philo_profile *profile, t_philo_manager *manager);

#endif
