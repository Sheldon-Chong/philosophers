#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdbool.h>

#define MSG_SLEEPING "is sleeping"
#define MSG_EATING "is eating"
#define MSG_THINKING "is thinking"
#define MSG_FORK "has taken a fork"
#define MSG_DEAD "died"

typedef struct s_philo	t_philo;

typedef struct s_session
{
	int					num_philos;
	unsigned long long	time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					num_philo_must_eat;
	unsigned long long	start_time;
	pthread_mutex_t		go_lock;
	pthread_mutex_t		die_lock;
	char				program_status;
	t_philo				*philos;
	pthread_mutex_t		*forks;
	pthread_t			*pid;
}	t_session;

typedef struct s_philo
{
	pthread_t			*pid;
	int					id;
	int					eat_count;
	unsigned long long	time_to_die;
	pthread_mutex_t		*fork_left;
	pthread_mutex_t		*fork_right;
	t_session			*session;
	pthread_mutex_t		eat_lock;
}	t_philo;

typedef struct s_supervisor
{
	pthread_t		*pid;
	int				i;
}	t_supervisor;


int	ft_atoi(char *c)
{
	int	i;
	int	ret;

	ret = 0;
	i = -1;
	while (c[++i])
	{
		ret *= 10;
		ret += c[i] - '0';
	}
	return (ret);
}

unsigned long long	get_time_milisec(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	ft_usleep(unsigned long long milisec)
{
	unsigned long long	start;

	start = 0;
	start = get_time_milisec();
	while (get_time_milisec() < (milisec + start))
		usleep(100);
}
