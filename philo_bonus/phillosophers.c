/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phillosophers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 10:36:09 by shechong          #+#    #+#             */
/*   Updated: 2024/02/29 08:25:19 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	parent(int philo_pid[200], t_session *session)
{
	int			i;
	int			status;
	pid_t		term_pid;
	pthread_t	eat_done;

	int pid = fork();
	if(pid == 0)
	{
		int eat_dones = 0;
		ft_usleep(50);
		while (1)
		{
			//printf("------------------------------------------------------------------------------ eaten %d\n", eat_dones);
			sem_wait(session->eat_done);
			eat_dones ++;
			if (eat_dones >= session->num_philos)
			{
				sem_post(session->all_philos_must_die);
				break;
			}
		}
		return ;
	}
	else
	{
		session->philo_pid = philo_pid;
		sem_post(session->go_lock);
		sem_wait(session->all_philos_must_die);
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

	sem_wait(session->go_lock);
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
	session->death_lock = sem_open("death_lock", O_CREAT, 0666, 1);
	sem_unlink("/go_lock");
	sem_unlink("/forks");
	sem_unlink("/eat_done");
	sem_unlink("/all_philos_must_die");
	session->all_philos_must_die = sem_open("/all_philos_must_die", O_CREAT, 0666, 1);
	sem_wait(session->all_philos_must_die);
	session->go_lock = sem_open("/go_lock", O_CREAT, 0666, 1);
	session->forks = sem_open("/forks", O_CREAT, 0666, session->num_philos);
	session->eat_done = sem_open("/eat_done",
			O_CREAT, 0666, session->num_philos);
	create_philos(session);
	sem_close(session->death_lock);
	sem_close(session->forks);
	sem_close(session->eat_done);
	free(session);
	return (0);
}
