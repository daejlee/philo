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
# include <stdlib.h>

typedef struct s_philo_profile
{
	int				idx;
	__uint64_t		r_eat;
	__uint64_t		r_sleep;
	__uint64_t		r_think;
	int				die_time;
	int				eat_time;
	int				sleep_time;
	sem_t			*t_sem;
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
	int				philo_num;
	int				must_eat_times;
	pid_t			*pid_arr;
	sem_t			*f_sem;
	sem_t			*m_sem;
	sem_t			*t_sem;
}	t_philo_manager;

int		ft_atoi(const char *nptr);
int		prep_args(t_philo_args *p, char **argv);
void	is_termination(t_philo_profile *profile, struct timeval *time);
int		grab_eat_sleep(t_philo_profile *p,
			struct timeval *time, t_philo_manager *manager);
void	init_profile(t_philo_profile *profile, t_philo_args args, sem_t *t_sem);
int		init_manager(t_philo_manager *manager, t_philo_args args);
int		free_mem(t_philo_manager *manager);

#endif
