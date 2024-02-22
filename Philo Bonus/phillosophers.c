/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phillosophers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 10:36:09 by shechong          #+#    #+#             */
/*   Updated: 2024/02/22 12:30:35 by shechong         ###   ########.fr       */
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
		sem_wait(philo->session->death_lock);
		if(philo->session->program_status == 'f')
			exit(0);
		if (get_time_milisec() > philo->time_to_die)
		{
			philo->session->program_status = 'd';
			printf("%llu %d %s\n", get_time_milisec()
				- philo->session->start_time, philo->id, MSG_DEAD);
			exit(0);
		}
		sem_post(philo->session->death_lock);
	}
	return (NULL);
}

void philo(int pid, t_session *session, int i)
{
	t_philo	philo;
	pthread_t thread;
	
	pthread_create(&thread, NULL, thread_reaper, &philo);
	pthread_detach(thread);
	philo.id = i + 1;
	philo.eat_count = 0;
	philo.session = session;
	philo.time_to_die = get_time_milisec() + session->time_to_die;
	while (1)
	{
		sem_wait(session->forks);
		print_message(MSG_FORK, &philo, session);
		sem_wait(session->forks);
		print_message(MSG_FORK, &philo, session);
		print_message(MSG_EATING, &philo, session);
		sem_wait(philo.session->death_lock);
		philo.eat_count ++;
		philo.time_to_die = get_time_milisec() + session->time_to_die;
		if (philo.eat_count >= session->num_philo_must_eat)
		{
			philo.session->program_status = 'f';
			exit(2);
		}
		sem_post(philo.session->death_lock);
		ft_usleep(session->time_to_eat);
		sem_post(session->forks);
		sem_post(session->forks);
		print_message(MSG_SLEEPING, &philo, session);
		ft_usleep(session->time_to_sleep);
		print_message(MSG_THINKING, &philo, session);
	}
	exit(1);
}

void parent(int	philo_pid[200], t_session *session)
{
	int		i;
	int		status;
	pid_t	term_pid;
	
	term_pid = waitpid(-1, &status, 0);

	i = -1;
	if (WEXITSTATUS(status) == 1)
	{
		i = -1;
		while(++i < session->num_philos)
			kill(philo_pid[i], SIGKILL);
	}
	i = -1;
}

void *create_philos(int count, t_session *session)
{
	int	i;
	int	pid;
	int	philo_pid[200];
	
	i = -1;
	while(++i < session->num_philos)
	{
		pid = fork();
		if (pid != 0)
			philo_pid[i] = pid;
		else
			break;
	}
	if (pid == 0)
		philo(pid, session, i);
	else
		parent(philo_pid, session);
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
	session->time_to_sleep = ft_atoi(av[4]);
	session->start_time = get_time_milisec();
	if (session->num_philos < 1)
		exit(printf("Error: Must have atleast 1 philo\n"));
	session->program_status = '\0';
	session->num_philo_must_eat = -1;
	if (ac > 5)
		session->num_philo_must_eat = ft_atoi(av[5]);
	return (session);
}

int main(int ac, char **av) {
	t_session *session;

	if (ac > 4)
		session = program_init(ac, av);
	else
		return (printf("Error: Not enough arguments\n"), 1);
	sem_unlink("death_lock");
	session->death_lock = sem_open("death_lock", O_CREAT | O_EXCL, 0666, 1);
	sem_unlink("forks");
	session->forks = sem_open("forks", O_CREAT, 0666, session->num_philos);
	create_philos(session->num_philos, session);
	sem_close(session->death_lock);
	sem_close(session->forks);
	sem_unlink("hello");
	sem_unlink("fork");
	free(session);
	return 0;
}
