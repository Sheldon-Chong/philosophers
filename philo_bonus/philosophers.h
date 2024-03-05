/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 10:37:41 by shechong          #+#    #+#             */
/*   Updated: 2024/03/05 09:34:51 by shechong         ###   ########.fr       */
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
	sem_t				*eat_done;
	int					*philo_pid;
	sem_t				*all_must_stop;
}	t_session;

typedef struct s_philo
{
	int					pid;
	int					id;
	int					eat_count;
	_Atomic uint64_t	lifespan;
	t_session			*session;
	sem_t				*read_lock;
	char				status;
}	t_philo;

void				print_msg(char *msg, t_philo *philo,
						t_session *session);
void				philo(int pid, t_session *session, int id);
int					ft_atoi(char *c);
unsigned long long	get_time_milisec(void);
int					ft_usleep(unsigned long long milisec);

#endif
