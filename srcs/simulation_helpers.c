#include "philosophers.h"

void	thinking(t_philosopher *phi)
{
	struct timeval	time_action;
	int				time;

	if (gettimeofday(&time_action, NULL))
		throw_error(ERROR_TIMEOFDAY);
	time = (int)(time_action.tv_sec - phi->times->start_time->tv_sec) *1000
		+ (time_action.tv_usec - phi->times->start_time->tv_usec) * 0.001;
	printf("%dms %d is thinking\n", time, phi->number);
}

void	ft_usleep(unsigned int n)
{
	struct timeval	start;
	struct timeval	step;

	gettimeofday(&start, NULL);
	while (1)
	{
		usleep(50);
		gettimeofday(&step, NULL);
		if ((unsigned int)(((int)(step.tv_sec - start.tv_sec)) * 1000000 +
			((unsigned int)(step.tv_usec - start.tv_usec))) > n)
			break ;
	}
}
