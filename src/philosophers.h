/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 10:37:41 by shechong          #+#    #+#             */
/*   Updated: 2024/02/19 19:32:12 by shechong         ###   ########.fr       */
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

void				print_message(char *msg, t_philo *philo,
						t_session *session);
void				*reaper_thread(void *data);
void				*thread_function(void *arg);
int					ft_atoi(char *c);
unsigned long long	get_time_milisec(void);
int					ft_usleep(unsigned long long milisec);

#endif
