/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 10:36:27 by shechong          #+#    #+#             */
/*   Updated: 2024/03/08 09:00:11 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	*thread_reaper(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		sem_wait(philo->session->death_lock);
		if (get_time_milisec() > philo->lifespan)
			break ;
		sem_post(philo->session->death_lock);
		usleep(1000);
	}
	printf("%llu %d %s\n", get_time_milisec()
		- philo->session->start_time, philo->id, MSG_DEAD);
	sem_post(philo->session->all_must_stop);
	exit(1);
}

static void	philo_life(t_philo *philo, t_session *session)
{
	sem_wait(session->forks);
	print_msg(MSG_FORK, philo, session);
	sem_wait(session->forks);
	print_msg(MSG_FORK, philo, session);
	print_msg(MSG_EATING, philo, session);
	philo->eat_count ++;
	if (session->num_philo_must_eat != -1 && philo->eat_count
		>= session->num_philo_must_eat)
		sem_post(philo->session->eat_done);
	sem_wait(session->death_lock);
	philo->lifespan = get_time_milisec() + session->time_to_die;
	sem_post(session->death_lock);
	ft_usleep(session->time_to_eat);
	sem_post(session->forks);
	sem_post(session->forks);
	print_msg(MSG_SLEEPING, philo, session);
	ft_usleep(session->time_to_sleep);
	print_msg(MSG_THINKING, philo, session);
}

void	philo(int pid, t_session *session, int id)
{
	t_philo		philo;
	pthread_t	reaper;

	philo.pid = pid;
	sem_wait(session->eat_done);
	philo.id = id + 1;
	philo.eat_count = 0;
	philo.session = session;
	philo.lifespan = get_time_milisec() + session->time_to_die;
	pthread_create(&reaper, NULL, thread_reaper, &philo);
	pthread_detach(reaper);
	if (philo.id % 2 != 0)
		ft_usleep(session->time_to_sleep);
	while (1)
		philo_life(&philo, session);
}
