/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgale <kgale@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/16 18:58:31 by lmartin           #+#    #+#             */
/*   Updated: 2021/07/12 15:44:53 by kgale            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	throw_error(int error)
{
	if (error == TOO_MANY_ARGS)
		printf("Error: Wrong number of arguments\n");
	else if (error == WRONG_ARG)
		printf("Error: Wrong argument\n");
	else if (error == ERROR_MALLOC)
		printf("Error: malloc error\n");
	else if (error == ERROR_MUTEX)
		printf("Error: mutex error\n");
	else if (error == ERROR_PTHREAD)
		printf("Error: pthread error\n");
	else if (error == ERROR_SLEEP)
		printf("Error: usleep error\n");
	else if (error == ERROR_TIMEOFDAY)
		printf("Error: gettimeofday error\n");
}

void	wait_philosophers(t_params *phi)
{
	int				c;
	t_philosopher	*ptr;

	ptr = phi->philosophers;
	while (ptr && !usleep(1000))
	{
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
		if (!(ptr = ptr->next) && c != phi->times->philo_number)
		{
			c *= 0;
			if (!c)
				ptr = phi->philosophers;
			else
				ptr = NULL;
		}
	}
}

void	clean_philosopher(t_philosopher *phi)
{
	if (phi->left_fork)
	{
		pthread_mutex_destroy(phi->left_fork->mutex);
		free(phi->left_fork->mutex);
		free(phi->left_fork);
	}
	free(phi->thread);

	free(phi);
	phi = NULL;
}
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
//	exit(-1);
}

void	unmake_pairs(t_params *phi)
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
	unmake_pairs(&params);
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
