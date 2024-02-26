/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 10:36:18 by shechong          #+#    #+#             */
/*   Updated: 2024/02/26 12:21:35 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_atoi(char *c)
{
	int	i;
	int	ret;

	ret = 0;
	i = -1;
	if (c[0] == '-')
		exit(printf("Error: %s: Negative value not accepted\n", c));
	while (c[++i])
	{
		if (!(c[i] >= '0' && c[i] <= '9'))
			exit(printf("Error: Invalid character found\n"));
		ret *= 10;
		ret += c[i] - '0';
	}
	return (ret);
}

unsigned long long	get_time_milisec(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	ft_usleep(unsigned long long milisec)
{
	unsigned long long	start;

	start = 0;
	start = get_time_milisec();
	while (get_time_milisec() < (milisec + start))
		usleep(100);
	return (1);
}

void	print_message(char *msg, t_philo *philo, t_session *session)
{
	unsigned long long	time_elapsed;

	sem_wait(philo->session->death_lock);
	time_elapsed = get_time_milisec() - session->start_time;
	printf("%llu %d %s\n", time_elapsed, philo->id, msg);
	sem_post(philo->session->death_lock);
}
