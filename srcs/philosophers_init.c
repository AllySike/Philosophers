#include "../includes/philosophers.h"

static void	ft_malloc(t_philosopher *tmp,
t_philosopher	*prev, int i, t_params *params)
{
	tmp->last_meal_time = malloc(sizeof(struct timeval));
	tmp->thread = malloc(sizeof(pthread_t));
	if (prev)
		tmp->left_fork = prev->right_fork;
	else
		tmp->left_fork = malloc (sizeof(t_fork));
	if (i == params->philo_number)
		tmp->right_fork = params->philosophers->left_fork;
	else
		tmp->right_fork = malloc(sizeof(t_fork));
	if (!tmp->thread || !tmp->right_fork || !tmp->left_fork
		|| !tmp->last_meal_time)
		exit(-1);// function
	tmp->right_fork->fork_mutex = malloc(sizeof(pthread_mutex_t));
	tmp->last_meal_mutex = malloc(sizeof(pthread_mutex_t));
	if (!tmp->right_fork->fork_mutex || !tmp->last_meal_mutex
		|| pthread_mutex_init(tmp->right_fork->fork_mutex, NULL)
		|| pthread_mutex_init(tmp->last_meal_mutex, NULL))
		exit(-1);//should be a function
	prev = tmp;
}

void	ft_init_philosophers(t_params *params)
{
	int				i;
	t_philosopher	*tmp;
	t_philosopher	*prev;

	i = 1;
	prev = NULL;
	params->start_time = malloc(sizeof(struct timeval));
	if (!params->start_time)
	{
		printf("Error with malloc\n");
		exit(-1);
	}
	gettimeofday(params->start_time, NULL);
	tmp = params->philosophers;
	while (i <= params->philo_number)
	{
		ft_malloc(tmp, prev, i, params);
		tmp->number = i;
		tmp->meal_count = 0;
		tmp->last_meal_time->tv_sec = params->start_time->tv_sec;
		tmp->last_meal_time->tv_usec = params->start_time->tv_usec;
		tmp->left_fork->last_numb = 0;
		tmp->next = NULL;
		if (i != params->philo_number)
			tmp->next = malloc(sizeof(t_philosopher));
		if (!tmp)
		{
			printf("Error with malloc\n");
			exit(-1);
		}
		params->curr = tmp;
		pthread_create(tmp->thread, NULL, simulation, params);
		tmp = tmp->next;
		i++;
	}
}
