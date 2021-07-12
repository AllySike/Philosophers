#include "philosophers.h"

int	eating(t_philosopher *phi)
{
	if (!phi->last_meal_time)
		return (1);
	if (gettimeofday(phi->last_meal_time, NULL))
		printf("Error with getting time\n");
	print_res("is eating", phi->number, phi->last_meal_time,
		phi->times->start_time);
	phi->meal_count++;
	ft_usleep(phi->times->eat_time * 1000);
	if (pthread_mutex_unlock(phi->right_fork->mutex))
		printf("Error with mutex\n");
	if (pthread_mutex_unlock(phi->left_fork->mutex))
		printf("Error with getting time\n");
	return (0);
}

int	check_eating(t_philosopher *phi)
{
	int				ret;
	struct timeval	time_action;

	if (gettimeofday(&time_action, NULL))
		printf("Error with getting time\n");
	if (!phi->last_meal_time || ((int)((time_action.tv_sec
				- phi->last_meal_time->tv_sec) * 1000
			+ (time_action.tv_usec - phi->last_meal_time->tv_usec)
			* 0.001) > phi->times->die_time))
	{
		free(phi->last_meal_time);
		print_res("died", phi->number, &time_action,
			phi->times->start_time);
		phi->last_meal_time = NULL;
	}
	ret = eating(phi);
	return (ret);
}

void	taking_forks(t_philosopher *phi)
{
	int				i;
	t_fork			*fork;
	struct timeval	time_act;

	i = 0;
	fork = phi->right_fork;
	if ((phi->number + i) % 2)
		fork = phi->left_fork;
	while (i < 2)
	{
		if (fork->last_numb != phi->number)
		{
			if (pthread_mutex_lock(fork->mutex) || gettimeofday(&time_act, 0))
				printf("Error\n");
			print_res("has taken a fork", phi->number, &time_act,
				phi->times->start_time);
			fork->last_numb = phi->number;
			if (++i != 2)
			{
				fork = phi->right_fork;
				if ((phi->number + i) % 2)
					fork = phi->left_fork;
			}
		}
	}
}

int	routine(t_philosopher *phi)
{
	int				ret;
	struct timeval	time_action;

	ret = check_eating(phi);
	if (ret || (phi->meal_count == phi->times->meal_number))
		return (1);
	if (gettimeofday(&time_action, NULL))
		printf("Error with getting time\n");
	print_res("is sleeping", phi->number, &time_action,
		phi->times->start_time);
	ft_usleep(phi->times->sleep_time * 1000);
	if (!phi->last_meal_time)
		return (1);
	if (gettimeofday(&time_action, NULL))
		printf("Error with getting time\n");
	print_res("is thinking", phi->number, &time_action,
		phi->times->start_time);
	return (0);
}

void	*simulation(void *args)
{
	t_philosopher	*phi;

	phi = (t_philosopher *)args;
	taking_forks(phi);
	while (!routine(phi))
		taking_forks(phi);
	return (NULL);
}
