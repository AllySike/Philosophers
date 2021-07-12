#include "philosophers.h"

void	ft_usleep(unsigned int n)
{
	struct timeval	start;
	struct timeval	step;

	gettimeofday(&start, NULL);
	while (1)
	{
		usleep(50);
		gettimeofday(&step, NULL);
		if ((int)(((int)(step.tv_sec - start.tv_sec)) * 1000000 +
			((int)(step.tv_usec - start.tv_usec))) > n)
			break ;
	}
}

void	print_res(char *line, int number,
struct timeval *t1, struct timeval *t2)
{
	int	time;

	time = (int)(t1->tv_sec
			- t2->tv_sec) *1000
		+ (t1->tv_usec
			- t2->tv_usec) * 0.001;
	printf("%dms %d %s\n", time, number, line);
}
