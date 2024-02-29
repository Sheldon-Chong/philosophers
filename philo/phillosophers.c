/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phillosophers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 10:36:09 by shechong          #+#    #+#             */
/*   Updated: 2024/02/26 14:08:30 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	print_msg(char *msg, t_philo *philo, t_session *session)
{
	unsigned long long	time_elapsed;

	pthread_mutex_lock(&(session->die_lock));
	time_elapsed = get_time_milisec() - session->start_time;
	if (msg[0] == 'd')
		printf("%llu %d died\n", time_elapsed, philo->id);
	else if (session->program_status == 0)
		printf("%llu %d %s\n", time_elapsed, philo->id, msg);
	pthread_mutex_unlock(&(session->die_lock));
}

t_philo	*create_philos(int count, t_session *session)
{
	int		i;
	t_philo	*philos;

	if (count < 1)
		return (NULL);
	philos = malloc(sizeof(t_philo) * (count + 1));
	session->pid = malloc(sizeof(pthread_t) * (count + 1));
	if (!philos || !session->pid)
		return (NULL);
	i = -1;
	while (++ i < count)
	{
		philos[i].pid = &session->pid[i];
		philos[i].id = i + 1;
		philos[i].eat_count = 0;
		philos[i].time_to_die = get_time_milisec() + session->time_to_die;
		philos[i].fork_left = &session->forks[i];
		philos[i].fork_right = &session->forks[(i + 1) % count];
		philos[i].session = session;
		pthread_mutex_init(&philos[i].eat_lock, NULL);
		pthread_create(&session->pid[i], NULL,
			philo_thread, (void *)&philos[i]);
	}
	return (philos);
}

t_session	*program_init(int ac, char **av)
{
	t_session	*session;
	int			i;

	session = malloc(sizeof(t_session));
	session->num_philos = ft_atoi(av[1]);
	session->time_to_die = ft_atoi(av[2]);
	session->time_to_eat = ft_atoi(av[3]);
	session->time_to_sleep = ft_atoi(av[4]);
	session->start_time = get_time_milisec();
	session->philos_eaten = 0;
	if (session->num_philos < 1 || session->num_philos > 200)
		exit(printf("Error: Must have between 1-200 philos\n"));
	session->program_status = '\0';
	session->forks = malloc((session->num_philos + 1)
			* sizeof(pthread_mutex_t));
	i = -1;
	while (++i < session->num_philos)
		pthread_mutex_init(&session->forks[i], NULL);
	pthread_mutex_init(&(session->go_lock), NULL);
	pthread_mutex_init(&(session->die_lock), NULL);
	session->num_philo_must_eat = -1;
	if (ac > 5)
		session->num_philo_must_eat = ft_atoi(av[5]);
	return (session);
}

void	cleanup(t_session *session)
{
	int	i;

	i = -1;
	free(session->philos);
	while (++i < session->num_philos)
		pthread_mutex_destroy(&session->forks[i]);
	pthread_mutex_destroy(&session->die_lock);
	pthread_mutex_destroy(&session->go_lock);
	free(session->forks);
	free(session->pid);
	free(session);
	exit(0);
}

int	main(int ac, char **av)
{
	t_session	*session;
	pthread_t	*reaper;
	int			i;

	if (ac > 4)
		session = program_init(ac, av);
	else
		return (printf("Error: Not enough arguments\n"), 1);
	pthread_mutex_lock(&(session->go_lock));
	session->philos = create_philos(session->num_philos, session);
	reaper = malloc(sizeof(pthread_t));
	pthread_create(reaper, NULL, reaper_thread, (void *)session);
	pthread_mutex_unlock(&(session->go_lock));
	i = -1;
	while (++i < session->num_philos)
		pthread_join(*(((t_philo *)session->philos)[i].pid), NULL);
	pthread_join(*reaper, NULL);
	cleanup(session);
}
