/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phillosophers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 10:36:09 by shechong          #+#    #+#             */
/*   Updated: 2024/02/20 12:25:01 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	print_message(char *msg, t_philo *philo, t_session *session)
{
	unsigned long long	time_elapsed;

	sem_wait(philo->session->death_lock);
	time_elapsed = get_time_milisec() - session->start_time;
	
		printf("%llu %d %s\n", time_elapsed, philo->id, msg);
	sem_post(philo->session->death_lock);
}

void* thread_reaper(void* arg) 
{
	t_philo	*philo = (t_philo *)arg;

	while (ft_usleep(10))
	{
		pthread_mutex_lock(&philo->eat_lock);
		if (get_time_milisec() > philo->time_to_die)
		{
			sem_wait(philo->session->death_lock);
			philo->session->program_status = 'd';
			sem_post(philo->session->death_lock);
			pthread_mutex_unlock(&philo->eat_lock);
			return(NULL);
		}
		pthread_mutex_unlock(&philo->eat_lock);
	}
	return NULL;
}

void *create_philos(int count, t_session *session)
{
	int		i;
	
	
	int	pid;
	int philo_pid[200];
	
	i = -1;
	while(++i < session->num_philos)
	{
		pid = fork();
		if (pid != 0)
		{
			philo_pid[i] = pid;
		}
		else if (pid == 0)
		{
			t_philo	philo;
			pthread_t thread;
			pthread_create(&thread, NULL, thread_reaper, &philo);
			philo.id = i + 1;
			philo.eat_count = 0;
			philo.session = session;
			philo.time_to_die = get_time_milisec() + session->time_to_die;
			pthread_mutex_init(&philo.eat_lock, NULL);
			while(1)
			{
				
				sem_wait(session->death_lock);
				if(session->program_status == 'd')
					{sem_post(session->death_lock);break;}
				sem_post(session->death_lock);
				sem_wait(session->forks);
				print_message(MSG_FORK, &philo, session);
				sem_wait(session->forks);
				print_message(MSG_FORK, &philo, session);
				pthread_mutex_lock(&philo.eat_lock);
				philo.time_to_die = get_time_milisec() + session->time_to_die;
				pthread_mutex_unlock(&philo.eat_lock);
				ft_usleep(session->time_to_eat);
				sem_post(session->forks);
				sem_post(session->forks);
				print_message(MSG_SLEEPING, &philo, session);
				ft_usleep(session->time_to_sleep);
				print_message(MSG_THINKING, &philo, session);
			}
			pthread_join(thread, NULL);
			printf("ended\n");
			return(NULL);
		}
		
	}
	/*
	
	KILL processes here
	
	*/

	i = -1;
	while(++i < session->num_philos)
		{printf(">> %d\n", philo_pid[i]);
		kill(philo_pid[i], SIGKILL);}
	return (NULL);
}

t_session	*program_init(int ac, char **av)
{
	t_session	*session;
	int			i;

	session = malloc(sizeof(t_session));
	session->num_philos = ft_atoi(av[1]);
	session->time_to_die = ft_atoi(av[2]);
	session->time_to_eat = ft_atoi(av[3]);
	session->time_to_die = ft_atoi(av[4]);
	session->start_time = get_time_milisec();
	if (session->num_philos < 1)
		exit(printf("Error: Must have atleast 1 philo\n"));
	session->program_status = '\0';
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
	pthread_mutex_destroy(&session->die_lock);
	pthread_mutex_destroy(&session->go_lock);
	free(session);
	exit(0);
}

int main(int ac, char **av) {
	t_session *session;

	if (ac > 4)
		session = program_init(ac, av);
	else
		return (printf("Error: Not enough arguments\n"), 1);
	sem_unlink("deat_lock");
	session->death_lock = sem_open("deat_lock", O_CREAT | O_EXCL, 0666, 1);
	
	sem_unlink("forks");
	session->forks = sem_open("forks", O_CREAT, 0666, session->num_philos);

	create_philos(session->num_philos, session);

	ft_usleep(10000);
	sem_close(session->death_lock);
	sem_close(session->forks);

	sem_unlink("hello");
	sem_unlink("fork");

	cleanup(session);

	return 0;
}
