#include "../includes/philosophers.h"

size_t			ft_strlen(const char *s)
{
	size_t i;

	i = 0;
	while (*s && ++i)
		s++;
	return (i);
}

void			fill_msg(char *msg, char **ptr)
{
	while (*msg)
		*(*ptr)++ = *msg++;
}

int		logs(struct timeval *st, struct timeval *tv, size_t number, char *msg)
{
	char		*ptr;
	char		*log;
	size_t		size_nb;
	long long	temp;
	long long	time;

	temp = number * 10;
	size_nb = 4 + ft_strlen(msg);
	while (temp /= 10)
		size_nb++;
	time = (tv->tv_sec - st->tv_sec) * 1000 +
(tv->tv_usec - st->tv_usec) * 0.001;
	temp = time;
	while (temp /= 10)
		size_nb++;
	if (!(log = malloc(sizeof(char) * (size_nb))))
		exit (-1);
	ptr = log;
	time = ft_atoi(ptr);
	fill_msg("ms ", &ptr);
	number = ft_atoi(ptr);\
	fill_msg(msg, &ptr);
	write(STDOUT_FILENO, log, size_nb);
	free(log);
	return (0);
}

int			taking_forks(t_params *params)
{
	int				ret;
	int				i;
	t_fork			*fork;
	t_philosopher	*phi;
	struct timeval	time_action;

	i = 0;
	phi = params->curr;
	if (!phi->last_meal_time)
		return (1);
	// fork = ((phi->number + i) % 2) ? phi->left_fork : phi->right_fork;
// 	while (i < 2)
// 	{
// 		if (fork->last_numb != phi->number)
// 		{
// 			if (pthread_mutex_lock(fork->fork_mutex))
// 				exit(-1);
// 			if (gettimeofday(&time_action, NULL))
// 				exit(-1);
// 			if ((ret = logs(params->start_time, &time_action, phi->number,
// " has taken a fork\n")))
// 				exit (-1);
// 			if ((fork->last_numb = phi->number) && ++i != 2)
// 				fork = ((phi->number + i) % 2) ? phi->left_fork : phi->right_fork;
// 		}
// 	}
	return (0);
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
		if ((size_t)(((size_t)(step.tv_sec - start.tv_sec)) * 1000000 +
((size_t)(step.tv_usec - start.tv_usec))) > n)
			break ;
	}
}

int			eating(t_params *params)
{
	int				ret;
	t_philosopher	*phi;

	if (!phi->last_meal_time)
	{
		if (pthread_mutex_unlock(phi->last_meal_mutex))
			exit(-1);
		return (1);
	}
	if (gettimeofday(phi->last_meal_time, NULL))
		exit(-1);
	if ((ret = logs(params->start_time, phi->last_meal_time,
phi->number, " is eating\n")))
		exit(-1);
	phi->meal_count++;
	ft_usleep(params->eat_time * 1000);
	if (pthread_mutex_unlock(phi->last_meal_mutex))
		exit(-1);
	return (0);
}

int			check_eating(t_params *params)
{
	int				ret;
	struct timeval	time_action;
	t_philosopher	*phi;

	phi = params->curr;
	if (gettimeofday(&time_action, NULL))
		exit(-1);
	if (pthread_mutex_lock(phi->last_meal_mutex))
		exit(-1);
	if (!phi->last_meal_time || ((size_t)((time_action.tv_sec -
phi->last_meal_time->tv_sec) * 1000 + (time_action.tv_usec -
phi->last_meal_time->tv_usec) * 0.001) > params->die_time))
	{
		free(phi->last_meal_time);
		if ((ret = logs(params->start_time,
&time_action, phi->number, " died\n")))
			exit(-1);
		phi->last_meal_time = NULL;
	}
	ret = eating(params);
	if (pthread_mutex_unlock(phi->right_fork->fork_mutex))
		exit(-1);
	if (pthread_mutex_unlock(phi->left_fork->fork_mutex))
		exit(-1);
	return (ret);
}

int			routine(t_params *params)
{
// 	int				ret;
// 	struct timeval	time_action;
// 	t_philosopher	*phi;

// 	phi = params->curr;
// 	if ((ret = check_eating(params)) || (phi->meal_count ==
// params->eat_time && (ret = 1)))
// 		return (ret);
// 	if (gettimeofday(&time_action, NULL))
// 		exit(-1);
// 	if ((ret = logs(params->start_time,
// &time_action, phi->number, " is sleeping\n")))
// 		exit (-1);
// 	ft_usleep(params->sleep_time * 1000);
// 	if (!phi->last_meal_time)
// 		return (1);
// 	if (gettimeofday(&time_action, NULL))
// 		exit (-1);
// 	if ((ret = logs(params->start_time,
// &time_action, phi->number, " is thinking\n")))
// 		exit (-1);
	return (0);
}

void	*simulation(void *args)
{
	t_params *params;

	params = (t_params *)args;
	while (!taking_forks(params) && !routine(params))
		NULL;
	return (NULL);
}
