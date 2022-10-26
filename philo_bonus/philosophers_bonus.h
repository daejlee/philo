#ifndef PHILOSOPHERS_BONUS_H
# define PHILOSOPHERS_BONUS_H
# include <pthread.h>
# include <sys/time.h>

/*
manager의 역할은 pid_t배열, f_sem, m_sem이 대체할 수 있을 듯.
각 철학자들은 프로세스다.
모든 프로세스의 생성마다 세마포어를 연다.
세마포어는 2개. -> 포크, must_eat
프로세스는 주어진 루틴 함수를 조건이 만족될 때 까지 무기한 실행한다.
조건이 만족되면(자식 중 하나의 프로세스가 종료되면) 다른 프로세스들도 모두 종료하고 메인 프로세스는 그들을 기다린다.
*/

#ifndef SEM_FORK
# define SEM_FORK "/myfork"
#endif

#ifndef SEM_MUST_EAT
# define SEM_MUST_EAT "/mymusteat"
#endif


typedef struct s_philo_profile
{
	__uint64_t		r_eat;
	__uint64_t		r_sleep;
	__uint64_t		r_think;
	int				die_time;
	int				eat_time;
	int				sleep_time;
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
	pid_t			*pid_arr;
	sem_t			*f_sem;
	sem_t			*m_sem;
	int				m_sem_flag;
}	t_philo_manager;

int		ft_atoi(const char *nptr);
int		prep_args(t_philo_args *p, char **argv);
int		is_termination(t_philo_profile *p_info, struct timeval *time);
int		grab_eat_sleep(t_philo_profile *p, struct timeval *time);
void	init_profile(t_philo_profile *profile, t_philo_args args);
int		init_manager(t_philo_manager *manager, t_philo_args args);
int		recover_thr_free_mem(t_philo_manager *manager, t_philo_args args);

#endif
