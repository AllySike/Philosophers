#include "philosophers.h"


int			eating(t_philosopher *phi)
{
	int		ret;
	int time;

	if (!phi->last_meal_time)
	{
//		if (pthread_mutex_unlock(phi->last_meal_mutex))
//			throw_error(ERROR_MUTEX);
		return (1);
	}
	if (gettimeofday(phi->last_meal_time, NULL))
		throw_error(ERROR_TIMEOFDAY);
	time = (int)(phi->last_meal_time->tv_sec - phi->times->start_time->tv_sec) *1000
	       + (phi->last_meal_time->tv_usec - phi->times->start_time->tv_usec) * 0.001;
	printf("%dms %d is esting\n", time, phi->number);
	phi->meal_count++;
	ft_usleep(phi->times->eat_time * 1000);
//	if (pthread_mutex_unlock(phi->last_meal_mutex))
//		throw_error(ERROR_MUTEX);
	return (0);
}

int			check_eating(t_philosopher *phi)
{
	int				ret;
	struct timeval	time_action;
	int time;

	if (gettimeofday(&time_action, NULL))
		throw_error(ERROR_TIMEOFDAY);
//	if (pthread_mutex_lock(phi->last_meal_mutex))
//		throw_error(ERROR_MUTEX);
	if (!phi->last_meal_time || ((size_t)((time_action.tv_sec -
	                                       phi->last_meal_time->tv_sec) * 1000 + (time_action.tv_usec -
	                                                                              phi->last_meal_time->tv_usec) * 0.001) > phi->times->die_time))
	{
		free(phi->last_meal_time);
		time = (int)(time_action.tv_sec - phi->times->start_time->tv_sec) *1000
		       + (time_action.tv_usec - phi->times->start_time->tv_usec) * 0.001;
		printf("%dms %d died\n", time, phi->number);
		phi->last_meal_time = NULL;
	}
	ret = eating(phi);
	if (pthread_mutex_unlock(phi->right_fork->mutex))
		throw_error(ERROR_MUTEX);
	if (pthread_mutex_unlock(phi->left_fork->mutex))
		throw_error(ERROR_MUTEX);
	return (ret);
}

void			 taking_forks(t_philosopher *phi)
{
	int				i;
	t_fork			*fork;
	struct timeval	time_action;
	int time;

	i = 0;
	fork = ((phi->number + i) % 2) ? phi->left_fork : phi->right_fork;
	while (i < 2)
	{
		if (fork->last_numb != phi->number)
		{
			if (pthread_mutex_lock(fork->mutex))
				throw_error(ERROR_MUTEX);
			if (gettimeofday(&time_action, NULL))
				throw_error(ERROR_TIMEOFDAY);
			time = (int)(time_action.tv_sec - phi->times->start_time->tv_sec) *1000
			       + (time_action.tv_usec - phi->times->start_time->tv_usec) * 0.001;
			printf("%dms %d has taken a fork\n", time, phi->number);
			if ((fork->last_numb = phi->number) && ++i != 2)
				fork = ((phi->number + i) % 2) ? phi->left_fork : phi->right_fork;
		}
	}
}

int			routine(t_philosopher *phi)
{
	int				ret;
	struct timeval	time_action;
	int time;

	if ((ret = check_eating(phi)) || (phi->meal_count ==
	                                  phi->times->meal_number && (ret = 1)))
		return (ret);
	if (gettimeofday(&time_action, NULL))
		throw_error(ERROR_TIMEOFDAY);
	time = (int)(time_action.tv_sec - phi->times->start_time->tv_sec) *1000
	       + (time_action.tv_usec - phi->times->start_time->tv_usec) * 0.001;
	printf("%dms %d is sleeping\n", time, phi->number);
	ft_usleep(phi->times->sleep_time * 1000);
	if (!phi->last_meal_time)
		return (1);
	if (gettimeofday(&time_action, NULL))
		throw_error(ERROR_TIMEOFDAY);
	time = (int)(time_action.tv_sec - phi->times->start_time->tv_sec) *1000
	       + (time_action.tv_usec - phi->times->start_time->tv_usec) * 0.001;
	printf("%dms %d is thinking\n", time, phi->number);
	return (0);
}

void		*simulation(void *args)
{
	t_philosopher	*phi;

	phi = (t_philosopher *)args;
	taking_forks(phi);
	while (!routine(phi))
		taking_forks(phi);
	return (NULL);
}
