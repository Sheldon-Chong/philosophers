#include "philosophers.h"


int	ft_atoi(char *c)
{
	int	i;
	int	ret;

	ret = 0;
	i = -1;
	while (c[++i])
	{
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
	return(1);
}
