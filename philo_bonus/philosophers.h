/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 10:37:41 by shechong          #+#    #+#             */
/*   Updated: 2024/02/26 12:23:14 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <pthread.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <stdbool.h>
# include <semaphore.h>
# include <fcntl.h>
# include <errno.h> 
# include <signal.h>

# define MSG_SLEEPING "is sleeping"
# define MSG_EATING "is eating"
# define MSG_THINKING "is thinking"
# define MSG_FORK "has taken a fork"
# define MSG_DEAD "died"

typedef struct s_philo	t_philo;

typedef struct s_session
{
	int					num_philos;
	unsigned long long	time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					num_philo_must_eat;
	unsigned long long	start_time;
	sem_t				*go_lock;
	char				program_status;
	t_philo				*philos;
	sem_t				*forks;
	sem_t				*death_lock;
	sem_t				*eat_lock;
	sem_t				*eat_done;
	int					*philo_pid;
}	t_session;

typedef struct s_philo
{
	int					pid;
	int					id;
	int					eat_done;
	unsigned long long	time_to_die;
	pthread_mutex_t		*fork_left;
	pthread_mutex_t		*fork_right;
	t_session			*session;
	pthread_mutex_t		eat_lock;
}	t_philo;

void				print_message(char *msg, t_philo *philo,
						t_session *session);
void				*thread_reaper(void *arg);
void				philo(int pid, t_session *session, int i);
int					ft_atoi(char *c);
unsigned long long	get_time_milisec(void);
int					ft_usleep(unsigned long long milisec);

#endif
