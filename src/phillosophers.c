#include "philosophers.h"

void	print_message(char *msg, t_philo *philo, t_session *session)
{
	unsigned long long	time_elapsed;

	pthread_mutex_lock(&(session->die_lock));
	time_elapsed = get_time_milisec() - session->start_time;
	if (msg[0] == 'd')
		printf("%llu %d died\n", time_elapsed, philo->id);
	if (session->program_status == 0)
		printf("%llu %d %s\n", time_elapsed, philo->id, msg);
	pthread_mutex_unlock(&(session->die_lock));
}

t_philo	*create_philos(int count, t_session *session)
{
	int			i;
	t_philo		*philos;

	if (count < 1)
		return (NULL);
	philos = malloc(sizeof(t_philo) * (count + 1));
	session->pid = malloc(sizeof(pthread_t) * (count + 1));
	if (!philos || !session->pid)
		return (NULL);
	i = -1;
	while (++ i < count)
	{
		philos[i] = (t_philo){&session->pid[i], i + 1, 0,
			get_time_milisec() + session->time_to_die,
			&session->forks[(i + 1) % count], &session->forks[i],
			session, {0, {0}}, };
		pthread_mutex_init(&philos[i].eat_lock, NULL);
		pthread_create(&session->pid[i], NULL,
			thread_function, (void *)&philos[i]);
	}
	return (philos);
}

t_session	*program_init(int ac, char **av)
{
	t_session	*session;
	int			i;

	session = malloc(sizeof(t_session));
	*session = (t_session){ft_atoi(av[1]), ft_atoi(av[2]), ft_atoi(av[3]),
		ft_atoi(av[4]), 0, get_time_milisec(), {0, {0}}, {0, {0}}, 0, 0, 0, 0};
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

void	end_simulation(t_session *session)
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
		return (printf("error\n"), 1);
	pthread_mutex_lock(&(session->go_lock));
	session->philos = create_philos(session->num_philos, session);
	reaper = malloc(sizeof(pthread_t));
	pthread_create(reaper, NULL, reaper_thread, (void *)session);
	pthread_mutex_unlock(&(session->go_lock));
	i = -1;
	while (++i < session->num_philos)
		pthread_join(*(((t_philo *)session->philos)[i].pid), NULL);
	pthread_join(*reaper, NULL);
}
