#include "philosophers.h"

static void	ft_malloc(t_philosopher *tmp,
t_philosopher	*prev, int i, t_params *params)
{
	tmp->last_meal_time = malloc(sizeof(struct timeval));
	tmp->thread = malloc(sizeof(pthread_t));
	if (prev)
		tmp->left_fork = prev->right_fork;
	else
		tmp->left_fork = malloc (sizeof(t_fork));
	if (i == params->times->philo_number)
		tmp->right_fork = params->philosophers->left_fork;
	else
		tmp->right_fork = malloc(sizeof(t_fork));
	if (!tmp->thread || !tmp->right_fork || !tmp->left_fork
		|| !tmp->last_meal_time)
		exit(-1);// function
	tmp->right_fork->mutex = malloc(sizeof(pthread_mutex_t));
	tmp->last_meal_mutex = malloc(sizeof(pthread_mutex_t));
	if (!tmp->right_fork->mutex || !tmp->last_meal_mutex
		|| pthread_mutex_init(tmp->right_fork->mutex, NULL)
		|| pthread_mutex_init(tmp->last_meal_mutex, NULL))
		exit(-1);//should be a function
}

static	void	helper(t_philosopher *tmp, t_params *params)
{
	int				i;
	t_philosopher	*prev;

	i = 1;
	prev = NULL;
	while (i <= params->times->philo_number)
	{
		ft_malloc(tmp, prev, i, params);
		if (!tmp)
			exit(-1);//printf("Error with malloc\n");
		tmp->left_fork->last_numb = 0;
		tmp->right_fork->last_numb = 0;
		tmp->number = i;
		tmp->meal_count = 0;
		tmp->last_meal_time->tv_sec = params->times->start_time->tv_sec;
		tmp->last_meal_time->tv_usec = params->times->start_time->tv_usec;
		tmp->left_fork->last_numb = 0;
		tmp->next = NULL;
		if (i != params->times->philo_number)
			tmp->next = malloc(sizeof(t_philosopher));
		prev = tmp;
		tmp->times = params->times;
		tmp = tmp->next;
		i++;
	}
}

void	ft_init_philosophers(t_params *params)
{
	t_philosopher	*tmp;

	params->times->start_time = malloc(sizeof(struct timeval));
	if (!params->times->start_time)
	{
		printf("Error with malloc\n");
		exit(-1);//
	}
	gettimeofday(params->times->start_time, NULL);
	tmp = params->philosophers;
	helper(tmp, params);
}
