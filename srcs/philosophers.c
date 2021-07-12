/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgale <kgale@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/12 18:55:02 by kgale             #+#    #+#             */
/*   Updated: 2021/07/12 19:42:36 by kgale            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	ft_error(t_params *params)
{
	printf("Error while executing\n");
	ft_exit(params);
	exit(0);
}

static void	wait_philosophers_utils(t_params *phi)
{
	int				c;
	t_philosopher	*ptr;

	c = 0;
	ptr = phi->philosophers;
	while (c < phi->times->philo_number)
	{
		if (!ptr->last_meal_time)
			c += 1;
		else
		{
			free(ptr->last_meal_time);
			ptr->last_meal_time = NULL;
		}
		ptr = ptr->next;
		if (!ptr && c != phi->times->philo_number)
		{
			c *= 0;
			ptr = phi->philosophers;
		}
	}
}

static void	wait_philosophers(t_params *phi)
{
	int				c;
	size_t			nb;
	t_philosopher	*ptr;

	nb = phi->times->meal_number;
	ptr = phi->philosophers;
	c = 0;
	while (ptr && !usleep(1000))
	{
		c += 1;
		if (ptr->meal_count < nb)
			c = 0;
		if (!ptr->last_meal_time || !ptr->next)
		{
			if (ptr->last_meal_time && c < phi->times->philo_number)
				ptr = phi->philosophers;
			else
				ptr = NULL;
		}
		else
			ptr = ptr->next;
	}
	wait_philosophers_utils(phi);
}

void	clean_philosopher(t_philosopher *phi)
{
	if (phi)
	{
		if (phi->left_fork)
		{
			pthread_mutex_destroy(phi->left_fork->mutex);
			free(phi->left_fork->mutex);
			free(phi->left_fork);
		}
		if (phi->thread)
			free(phi->thread);
		free(phi);
	}
	phi = NULL;
}

int	main(int argc, char *argv[])
{
	int				ret;
	t_params		params;
	t_philosopher	*tmp;

	if (argc < 5 || argc > 6)
	{
		printf("Error: Wrong number of arguments\n");
		return (-1);
	}
	ft_init_params(&params, argc, argv);
	if (!params.times->meal_number)
		ft_exit(&params);
	ft_init_philosophers(&params);
	tmp = params.philosophers;
	while (tmp)
	{
		pthread_create(tmp->thread, NULL, &simulation, tmp);
		tmp = tmp->next;
	}
	wait_philosophers(&params);
	usleep(100000);
	ft_exit(&params);
}
