#include "philosophers.h"

void end_program(t_session *session)
{
	
}

void	print_message(char c, t_philo *philo, t_session *session)
{
	unsigned long long time_elapsed;

	
	pthread_mutex_lock(&(session->die_lock));
	time_elapsed = get_time_milisec() - session->start_time;
	if(c == 'd')
		printf("%llu %d died\n", time_elapsed, philo->id);
	if (session->program_status == 0)
	{
		if (c == 'f')
			printf("%llu %d has taken a fork\n", time_elapsed, philo->id);
		else if(c == 'e')
			printf("%llu %d is eating %llu\n", time_elapsed, philo->id, philo->time_to_die - session->start_time);
		else if(c == 's')
			printf("%llu %d is sleeping\n", time_elapsed, philo->id);
		else if (c == 't')
			printf("%llu %d is thinking\n", time_elapsed, philo->id);
	}
	pthread_mutex_unlock(&(session->die_lock));
}

void take_forks (t_philo *philo)
{
	if (philo->id % 2 == 0) {
        pthread_mutex_lock(philo->fork_left);
        print_message('f', philo, philo->session);
        pthread_mutex_lock(philo->fork_right);
        print_message('f', philo, philo->session);
    } else {
        pthread_mutex_lock(philo->fork_right);
        print_message('f', philo, philo->session);
        pthread_mutex_lock(philo->fork_left);
        print_message('f', philo, philo->session);
    }
}

void *reaper_thread(void *data)
{
    t_session *session = (t_session *)data;
    t_philo *philos = (t_philo *)session->philos;
	pthread_mutex_lock(&session->go_lock);
    pthread_mutex_unlock(&session->go_lock);
    while(1)
    {
        
        int i = -1;
		pthread_mutex_lock(&(session->die_lock));
        while(++i < session->num_philos)
        {
            unsigned long long current_time = get_time_milisec();
            if(current_time > philos[i].time_to_die)
            {
                session->program_status = 'd';
               	printf("%llu %d died\n", get_time_milisec() - session->start_time, i);
                pthread_mutex_unlock(&(session->die_lock));
                return NULL;
            }
            if(session->program_status == 'e')
            {
                pthread_mutex_unlock(&(session->die_lock));
                return NULL;
            }
			
        }
        pthread_mutex_unlock(&(session->die_lock));
		ft_usleep(10);
    }
    return NULL;
}

void* thread_function(void* arg)
{
    t_philo	*philo;
    
    philo = ((t_philo*)arg);
    pthread_mutex_lock(philo->go_lock);
    pthread_mutex_unlock(philo->go_lock);
    if (philo->id % 2 != 0)
        ft_usleep(philo->session->time_to_eat);
    pthread_mutex_lock(&(philo->session->die_lock));
    philo->time_to_die = get_time_milisec() + philo->session->time_to_die;
    pthread_mutex_unlock(&(philo->session->die_lock));
    while(1)
    {
        pthread_mutex_lock(&(philo->session->die_lock));
        if(philo->session->program_status != 0)
        {
            pthread_mutex_unlock(&(philo->session->die_lock));
            break;
        }
        pthread_mutex_unlock(&(philo->session->die_lock));
        take_forks(philo);
        philo->eat_count++;
        if (philo->eat_count > 3)
        {
            pthread_mutex_lock(&(philo->session->die_lock));
            philo->session->program_status = 'e';
			pthread_mutex_unlock(philo->fork_left);
        	pthread_mutex_unlock(philo->fork_right);
            pthread_mutex_unlock(&(philo->session->die_lock));
			
			break;
        }
        print_message('e', philo, philo->session);
        pthread_mutex_lock(&(philo->session->die_lock));
        philo->time_to_die = get_time_milisec() + philo->session->time_to_die;
        pthread_mutex_unlock(&(philo->session->die_lock));
        ft_usleep(philo->session->time_to_eat);
        pthread_mutex_unlock(philo->fork_left);
        pthread_mutex_unlock(philo->fork_right);
        print_message('s', philo, philo->session);
        ft_usleep(philo->session->time_to_sleep);
        print_message('t', philo, philo->session);
    }
    return	NULL;
}

t_philo	*create_philos(int count, t_session *session)
{
	int				i;
	pthread_t		*pid;
	t_philo			*philos;

	if (count < 1)
		return (NULL);
	philos = malloc(sizeof(t_philo) * (count + 1));
	session->pid = malloc(sizeof(pthread_t) * (count + 1));
	if (!philos || !pid)
		return (NULL);
	i = -1;
	
	while (++ i < count)
	{
		philos[i] = (t_philo){
				&session->pid[i], 
				i + 1, 
				0, 
				get_time_milisec() + session->time_to_die,
				&session->forks[(i + 1) % count], 
				&session->forks[i], 
				session, 
				pthread_mutex_init(&(philos[i].eat_lock), NULL), 
				0,
			};
		philos[i].go_lock = &session->go_lock;
		pthread_create(&session->pid[i], NULL, thread_function, (void *)&philos[i]);
	}
	i = -1;
	while(++i < session->num_philos)
	{
		printf("%d | %p | left: %p, right: %p", philos[i].id, philos[i].pid, philos[i].fork_left, philos[i].fork_right);
		printf(" | time to die %llu %llu", philos[i].time_to_die, get_time_milisec());
		if (philos[i].id % 2 == 0)
			printf(" starts");
		printf("\n");
	}
	return (philos);
}

t_session *program_init(int ac, char **av)
{
	t_session	*session;
	int i;

	session = malloc(sizeof(t_session));
	*session = (t_session){ft_atoi(av[1]), ft_atoi(av[2]), ft_atoi(av[3]),
		ft_atoi(av[4]), 0, get_time_milisec(), 0, 0, '\0', 0};

	session->forks =  malloc((session->num_philos + 1) * sizeof(pthread_mutex_t));
	i = -1;
	while(++i < session->num_philos)
		pthread_mutex_init(&session->forks[i], NULL);
	
	pthread_mutex_init(&(session->go_lock), NULL);
	pthread_mutex_init(&(session->die_lock), NULL);
	if (ac > 5)
		session->num_philo_must_eat = ft_atoi(av[5]);
	return(session);
}

int	main(int ac, char **av)
{
	t_session	*session;
	t_philo		*philos;
	int i;

	if (ac > 4)
		session = program_init(ac, av);
	else
		return 0;
	
	printf("num_philos: %d\n", session->num_philos); printf("time_to_die %d\n", session->time_to_die); printf("time_to_eat %d\n", session->time_to_eat); printf("time_to_sleep %d\n", session->time_to_sleep); printf("num_philo_must_eat %d\n\n", session->num_philo_must_eat);
	pthread_mutex_lock(&(session->go_lock));
	philos = create_philos(session->num_philos, session);
	
	session->philos = philos;
	pthread_t *reaper = malloc(sizeof(pthread_t));
	
	pthread_create(reaper, NULL, reaper_thread, (void *)session);
	pthread_mutex_unlock(&(session->go_lock));
	i = -1;
	while(++i < session->num_philos)
		pthread_join(*(philos[i].pid), NULL);
	pthread_join(*reaper, NULL);

	

	i = -1;
	free(session->philos);
	while(++i < session->num_philos)
	{
		pthread_mutex_destroy(&session->forks[i]);
	}
	pthread_mutex_destroy(&session->die_lock);
	pthread_mutex_destroy(&session->go_lock);
	free(session->forks);
	free(session->pid);
	free(session);
	printf("exited\n");
	exit(0);
}

//change name from supervisor to reaper

/*
Should each process have their own reaper, or is a single reaper sufficient?


What does this mean?:
Test with 2 philosophers and check the different times (a death delayed by more than 10 ms is unacceptable).
*/