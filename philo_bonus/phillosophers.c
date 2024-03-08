/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phillosophers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 10:36:09 by shechong          #+#    #+#             */
/*   Updated: 2024/03/08 08:50:17 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	eat_checker(t_session *session)
{
	int	eat_dones;

	eat_dones = 0;
	ft_usleep(50);
	while (1)
	{
		sem_wait(session->eat_done);
		eat_dones ++;
		if (eat_dones >= session->num_philos)
			exit(sem_post(session->all_must_stop));
	}
}

void	parent(int philo_pid[200], t_session *session)
{
	int		i;
	int		pid;

	pid = fork();
	if (pid == 0)
		eat_checker(session);
	else
	{
		session->philo_pid = philo_pid;
		sem_wait(session->all_must_stop);
		i = -1;
		while (++i < session->num_philos)
			kill(session->philo_pid[i], SIGKILL);
		kill(pid, SIGKILL);
	}
}

void	*create_philos(t_session *session)
{
	int	i;
	int	pid;
	int	philo_pid[200];

	i = -1;
	while (++i < session->num_philos)
	{
		pid = fork();
		if (pid != 0)
			philo_pid[i] = pid;
		else
			break ;
	}
	if (pid == 0)
		philo(philo_pid[i], session, i);
	else
		parent(philo_pid, session);
	return (NULL);
}

t_session	*program_init(int ac, char **av)
{
	t_session	*session;

	session = malloc(sizeof(t_session));
	session->num_philos = ft_atoi(av[1]);
	session->time_to_die = ft_atoi(av[2]);
	session->time_to_eat = ft_atoi(av[3]);
	session->time_to_sleep = ft_atoi(av[4]);
	session->start_time = get_time_milisec();
	if (session->num_philos < 1 || session->num_philos > 200)
		exit(printf("Error: Must have between 1-200 philos\n"));
	session->program_status = '\0';
	session->num_philo_must_eat = -1;
	if (ac > 5)
		session->num_philo_must_eat = ft_atoi(av[5]);
	return (session);
}

int	main(int ac, char **av)
{
	t_session	*session;

	if (ac > 4)
		session = program_init(ac, av);
	else
		return (printf("Error: Not enough arguments\n"), 1);
	sem_unlink("death_lock");
	sem_unlink("/forks");
	sem_unlink("/eat_done");
	sem_unlink("/all_must_stop");
	session->death_lock = sem_open("death_lock", O_CREAT, 0644, 1);
	session->eat_done = sem_open("/eat_lock", O_CREAT, 0644, 1);
	session->all_must_stop = sem_open("/all_must_stop", O_CREAT, 0644, 1);
	session->forks = sem_open("/forks", O_CREAT, 0644, session->num_philos);
	session->eat_done = sem_open("/eat_done", O_CREAT,
			0644, session->num_philos);
	sem_wait(session->all_must_stop);
	create_philos(session);
	sem_close(session->death_lock);
	sem_close(session->forks);
	sem_close(session->eat_done);
	free(session);
	return (0);
}
