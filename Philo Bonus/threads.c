/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 10:36:27 by shechong          #+#    #+#             */
/*   Updated: 2024/02/20 11:03:56 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*reaper_loop(t_session *session)
{
	unsigned long long	current_time;
	int					i;

	while (ft_usleep(10))
	{
		i = -1;
		while (++i < session->num_philos)
		{
			pthread_mutex_lock(&(session->die_lock));
			current_time = get_time_milisec();
			if (current_time > session->philos[i].time_to_die)
			{
				session->program_status = 'd';
				return (pthread_mutex_unlock(&(session->die_lock)),
					print_message(MSG_DEAD, &session->philos[i],
						session->philos[i].session), NULL);
			}
			if (session->program_status == 'e')
				return (pthread_mutex_unlock(&(session->die_lock)), NULL);
			pthread_mutex_unlock(&(session->die_lock));
		}
	}
	return (NULL);
}

void	*reaper_thread(void *data)
{
	t_session			*session;

	session = (t_session *)data;
	pthread_mutex_lock(&session->go_lock);
	pthread_mutex_unlock(&session->go_lock);

	if (session->num_philos == 1)
	{
		print_message(MSG_FORK, &session->philos[0], session);
		while (get_time_milisec() < session->philos[0].time_to_die)
		{
		}
		return (print_message(MSG_DEAD,
				&session->philos[0], session), NULL);
	}
	reaper_loop(session);
	return (NULL);
}

int	philo_life(t_philo *philo)
{
	pthread_mutex_lock(philo->fork_right);
	print_message(MSG_FORK, philo, philo->session);
	pthread_mutex_lock(philo->fork_left);
	print_message(MSG_FORK, philo, philo->session);
	philo->eat_count++;
	print_message(MSG_EATING, philo, philo->session);
	if (philo->session->num_philo_must_eat > 0
		&& philo->eat_count > philo->session->num_philo_must_eat)
	{
		pthread_mutex_unlock(philo->fork_left);
		pthread_mutex_unlock(philo->fork_right);
		pthread_mutex_lock(&(philo->session->die_lock));
		philo->session->program_status = 'e';
		pthread_mutex_unlock(&(philo->session->die_lock));
		return (1);
	}
	pthread_mutex_lock(&(philo->session->die_lock));
	philo->time_to_die = get_time_milisec() + philo->session->time_to_die;
	pthread_mutex_unlock(&(philo->session->die_lock));
	ft_usleep(philo->session->time_to_eat);
	pthread_mutex_unlock(philo->fork_left);
	pthread_mutex_unlock(philo->fork_right);
	print_message(MSG_SLEEPING, philo, philo->session);
	ft_usleep(philo->session->time_to_sleep);
	return (print_message(MSG_THINKING, philo, philo->session), 0);
}

void	*philo_thread(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->session->go_lock);
	pthread_mutex_unlock(&philo->session->go_lock);

	if (philo->session->num_philos == 1)
		return (NULL);
	if (philo->id % 2 != 0)
		ft_usleep(philo->session->time_to_eat);
	pthread_mutex_lock(&(philo->session->die_lock));
	philo->time_to_die = get_time_milisec() + philo->session->time_to_die;
	pthread_mutex_unlock(&(philo->session->die_lock));
	while (1)
	{
		pthread_mutex_lock(&(philo->session->die_lock));
		if (philo->session->program_status != 0)
			return (pthread_mutex_unlock(&(philo->session->die_lock)), NULL);
		pthread_mutex_unlock(&(philo->session->die_lock));
		if (philo_life(philo))
			break ;
	}
	return (NULL);
}
