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