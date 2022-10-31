#ifndef PHILOSOPHERS_BONUS_H
# define PHILOSOPHERS_BONUS_H
# include <pthread.h>
# include <sys/time.h>
# include <semaphore.h>

/*
manager의 역할은 pid_t배열, f_sem, m_sem이 대체할 수 있을 듯.
각 철학자들은 프로세스다.
모든 프로세스의 생성마다 세마포어를 연다.
세마포어는 2개. -> 포크, must_eat
프로세스는 주어진 루틴 함수를 조건이 만족될 때 까지 무기한 실행한다.
조건이 만족되면(자식 중 하나의 프로세스가 종료되면) 다른 프로세스들도
모두 종료하고 메인 프로세스는 그들을 기다린다.
*/

#ifndef SEM_FORK
# define SEM_FORK "/mysem_fork"
#endif

#ifndef SEM_MUST_EAT
# define SEM_MUST_EAT "/mysem_musteat"
#endif

#ifndef SEM_TERMINATE
# define SEM_TERMINATE "/mysem_terminate"
#endif

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
	pid_t			*pid_arr;
	sem_t			*f_sem;
	sem_t			*m_sem;
	sem_t			*t_sem;
}	t_philo_manager;

int		ft_atoi(const char *nptr);
int		prep_args(t_philo_args *p, char **argv);
int		is_termination(t_philo_profile *profile, struct timeval *time, t_philo_manager *manager);
int		grab_eat_sleep(t_philo_profile *p, struct timeval *time, t_philo_manager *manager);
void	init_profile(t_philo_profile *profile, t_philo_args args, sem_t *t_sem);
int		init_manager(t_philo_manager *manager, t_philo_args args);
int		free_mem(t_philo_manager *manager);

#endif
