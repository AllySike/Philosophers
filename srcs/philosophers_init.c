/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_init.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgale <kgale@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/12 17:28:30 by kgale             #+#    #+#             */
/*   Updated: 2021/07/12 17:28:30 by kgale            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	ft_exit(t_params *phi)
{
	void			*temp;
	t_philosopher	*ptr;

	ptr = NULL;
	if (phi)
		ptr = phi->philosophers;
	while (ptr)
	{
		temp = ptr->next;
		clean_philosopher(ptr);
		ptr = temp;
	}
	if (phi && phi->times && phi->times->start_time)
		free(phi->times->start_time);
	if (phi && phi->times)
		free(phi->times);
}

static void	ft_malloc(t_philosopher *tmp,
t_philosopher	*prev, int i, t_params *params)
{
	tmp->last_meal_time = malloc(sizeof(struct timeval));
	tmp->thread = malloc(sizeof(pthread_t));
	if (prev)
		tmp->left_fork = prev->right_fork;
	else
		tmp->left_fork = malloc(sizeof(t_fork));
	if (i == params->times->philo_number)
		tmp->right_fork = params->philosophers->left_fork;
	else
		tmp->right_fork = malloc(sizeof(t_fork));
	if (!tmp->thread || !tmp->right_fork || !tmp->left_fork
		|| !tmp->last_meal_time)
		ft_error(params);
	tmp->right_fork->mutex = malloc(sizeof(pthread_mutex_t));
	if (!tmp->right_fork->mutex
		|| pthread_mutex_init(tmp->right_fork->mutex, NULL))
		ft_error(params);
}

static void	unmake_pairs(t_params *phi)
{
	t_philosopher	*temp;
	t_philosopher	*ptr;
	t_philosopher	*odd;
	t_philosopher	*even;

	ptr = phi->philosophers;
	odd = phi->philosophers;
	temp = phi->philosophers->next;
	even = phi->philosophers->next;
	while (ptr)
	{
		if (ptr->number % 2 && ptr->number != 1)
		{
			odd->next = ptr;
			odd = odd->next;
		}
		else if (!(ptr->number % 2) && ptr->number != 2)
		{
			even->next = ptr;
			even = even->next;
		}
		ptr = ptr->next;
	}
	odd->next = temp;
	even->next = NULL;
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
			ft_error(params);
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
		ft_error(params);
	gettimeofday(params->times->start_time, NULL);
	tmp = params->philosophers;
	helper(tmp, params);
	unmake_pairs(params);
}
