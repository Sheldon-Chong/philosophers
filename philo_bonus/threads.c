/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 10:36:27 by shechong          #+#    #+#             */
/*   Updated: 2024/02/29 10:00:52 by shechong         ###   ########.fr       */
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
		pthread_mutex_lock(&philo->eat_lock);
		if (get_time_milisec() > philo->time_to_die)
		{
			philo->session->program_status = 'd';
			printf("%llu %d %s\n", get_time_milisec()
				- philo->session->start_time, philo->id, MSG_DEAD);
			sem_post(philo->session->all_philos_must_die);
			exit(1);
		}
		pthread_mutex_unlock(&philo->eat_lock);
		sem_post(philo->session->death_lock);
		ft_usleep(10);
	}
	return (NULL);
}

static void	philo_life(t_philo *philo, t_session *session)
{
	sem_wait(session->forks);
	print_msg(MSG_FORK, philo, session);
	sem_wait(session->forks);
	print_msg(MSG_FORK, philo, session);
	philo->eat_done ++;
	print_msg(MSG_EATING, philo, session);
	if (philo->session->num_philo_must_eat != -1 && philo->eat_done
		>= session->num_philo_must_eat)
		sem_post(philo->session->eat_done);
	pthread_mutex_lock(&philo->eat_lock);
	philo->time_to_die = get_time_milisec() + session->time_to_die;
	pthread_mutex_unlock(&philo->eat_lock);
	ft_usleep(session->time_to_eat);
	sem_post(session->forks);
	sem_post(session->forks);
	print_msg(MSG_SLEEPING, philo, session);
	ft_usleep(session->time_to_sleep);
	print_msg(MSG_THINKING, philo, session);
}

void	philo(int pid, t_session *session, int i)
{
	t_philo		philo;
	pthread_t	thread;

	philo.pid = pid;
	sem_wait(session->eat_done);
	sem_wait(session->go_lock);
	sem_post(session->go_lock);
	philo.id = i + 1;
	philo.eat_done = 0;
	philo.session = session;
	pthread_mutex_init(&philo.eat_lock, NULL);
	philo.time_to_die = get_time_milisec() + session->time_to_die;
	pthread_create(&thread, NULL, thread_reaper, &philo);
	pthread_detach(thread);
	if (philo.id % 2 != 0)
		ft_usleep(session->time_to_eat);
	while (1)
		philo_life(&philo, session);
}
