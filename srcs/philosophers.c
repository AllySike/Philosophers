/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgale <kgale@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/16 18:58:31 by lmartin           #+#    #+#             */
/*   Updated: 2021/07/05 15:03:57 by kgale            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		if ((unsigned int)(((int)(step.tv_sec - start.tv_sec)) * 1000000 +
((unsigned int)(step.tv_usec - start.tv_usec))) > n)
			break ;
	}
}

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

int			eating(t_philosopher *phi)
{
	int     time;

	if (!phi->last_meal_time)
	{
		if (pthread_mutex_unlock(phi->last_meal_mutex))
			throw_error(ERROR_MUTEX);
		return (1);
	}
	if (gettimeofday(phi->last_meal_time, NULL))
		throw_error(ERROR_TIMEOFDAY);
	time = 	(int)((phi->last_meal_time->tv_sec - phi->times->start_time->tv_sec) * 1000 +
	                 (phi->last_meal_time->tv_usec - phi->times->start_time->tv_usec) * 0.001);
	printf("%dms %d is eating\n", time, phi->number);
	phi->meal_count++;
	ft_usleep(phi->times->eat_time * 1000);
	if (pthread_mutex_unlock(phi->last_meal_mutex))
		throw_error(ERROR_MUTEX);
	return (0);
}

void			check_dead(t_philosopher *phi)
{
	struct timeval	time_action;
	int time;

	if (gettimeofday(&time_action, NULL))
		throw_error(ERROR_TIMEOFDAY);
	if (pthread_mutex_lock(phi->last_meal_mutex))
		throw_error(ERROR_MUTEX);
	if (!phi->last_meal_time || ((int)((time_action.tv_sec -
	    phi->last_meal_time->tv_sec) * 1000 + (time_action.tv_usec -
	    phi->last_meal_time->tv_usec) * 0.001) > phi->times->die_time))
	{
		free(phi->last_meal_time);
		time = (time_action.tv_sec - phi->times->start_time->tv_sec) * 1000 +
			(time_action.tv_usec - phi->times->start_time->tv_usec) * 0.001;
		printf("%dms %d died\n", time, phi->number);
		phi->last_meal_time = NULL;
	}
}

void			taking_forks(t_philosopher *phi)
{
	int				i;
	t_fork			*fork;
	struct timeval	time_action;
	int             time;

	i = 1;
	if (phi->number % 2)
		fork = phi->left_fork;
	else
		fork = phi->right_fork;
	while (i <= 2 && phi->next)
	{
		if (pthread_mutex_lock(fork->fork_mutex))
			throw_error(ERROR_MUTEX);
		if (gettimeofday(&time_action, NULL))
			throw_error(ERROR_TIMEOFDAY);
		time = (time_action.tv_sec - phi->times->start_time->tv_sec) * 1000 +
			(time_action.tv_usec - phi->times->start_time->tv_usec) * 0.001;
		printf("%dms %d has taken a fork\n", time, phi->number);
		if ((fork->last_numb = phi->number) && i++ != 2)
		{
			if ((phi->number + i) % 2)
				fork = phi->right_fork;
			else
				fork = phi->left_fork;
		}
	}
}

void            thinking(t_philosopher *phi)
{
	struct timeval	time_action;
	int time;

	if (gettimeofday(&time_action, NULL))
		throw_error(ERROR_TIMEOFDAY);
	time = (int)(time_action.tv_sec - phi->times->start_time->tv_sec) * 1000 +
	       (time_action.tv_usec - phi->times->start_time->tv_usec) * 0.001;
	printf("%dms %d is thinking\n", time, phi->number);
}

int			routine(t_philosopher *phi)
{
	int				ret;
	struct timeval	time_action;
	int time;

	ret = eating(phi);
	if (pthread_mutex_unlock(phi->right_fork->fork_mutex))
		throw_error(ERROR_MUTEX);
	if (pthread_mutex_unlock(phi->left_fork->fork_mutex))
		throw_error(ERROR_MUTEX);
	if (ret || (phi->meal_count == phi->times->meal_number))
		return (ret);
	if (gettimeofday(&time_action, NULL))
		throw_error(ERROR_TIMEOFDAY);
	time = (int)(time_action.tv_sec - phi->times->start_time->tv_sec) * 1000 +
	       (time_action.tv_usec - phi->times->start_time->tv_usec) * 0.001;
	printf("%dms %d is sleeping\n", time, phi->number);
	ft_usleep(phi->times->sleep_time * 1000);
	if (!phi->last_meal_time)
		return (1);
	thinking(phi);
	return (0);
}

void		*simulation(void *args)
{
	t_philosopher	*phi;

	phi = (t_philosopher *)args;
	taking_forks(phi);
	check_dead(phi);
	while (!routine(phi)) {
		taking_forks(phi);
		check_dead(phi);
	}
	return (NULL);
}

int		wait_philosophers(t_params *phi)
{
	int				c;
	int			number;
	t_philosopher	*ptr;

	number = phi->times->meal_number;
	ptr = phi->philosophers;
	while (ptr && !usleep(1000) && (c = (ptr->meal_count < number) ? 0 : c + 1) > -2)
		if (!ptr->last_meal_time || !ptr->next)
			ptr = (ptr->last_meal_time &&
c < phi->times->philo_number) ? phi->philosophers : NULL;
		else
			ptr = ptr->next;
	c = 0;
	ptr = phi->philosophers;
	while (c < phi->times->philo_number)
	{
		if (pthread_mutex_lock(ptr->last_meal_mutex))
			return (ERROR_MUTEX);
		c += (!ptr->last_meal_time) ? 1 : !!(ptr->last_meal_time = NULL) + 0;
		if (pthread_mutex_unlock(ptr->last_meal_mutex))
			return (ERROR_MUTEX);
		if (!(ptr = ptr->next) && c != phi->times->philo_number)
			ptr = !(c *= 0) ? phi->philosophers : NULL;
	}
	return (0);
}

void	clean_philosopher(t_philosopher *phi)
{
	if (phi->left_fork && phi->left_fork->fork_mutex &&
pthread_mutex_destroy(phi->left_fork->fork_mutex))
		throw_error(ERROR_MUTEX);
	 if (phi->left_fork)
	 	free(phi->left_fork->fork_mutex);
	 free(phi->left_fork);
	if (phi->last_meal_mutex && pthread_mutex_destroy(phi->last_meal_mutex))
		throw_error(ERROR_MUTEX);
	 free(phi->last_meal_mutex);
	 free(phi->thread);
	 free(phi->last_meal_time);
	 free(phi);
}

int		clean_all(t_params *phi)
{
	void			*temp;
	t_philosopher	*ptr;

	ptr = (phi) ? phi->philosophers : NULL;
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
	return (0);
}

int		main(int argc, char *argv[])
{
	int				ret;
	t_params		params;
	t_philosopher *tmp;

	if (argc < 5 || argc > 6)
	{
		printf("Error: Wrong number of arguments\n");
		return (-1);
	}
	ft_init_params(&params, argc, argv);
	if (!params.times->meal_number)
		return (clean_all(&params));
	ft_init_philosophers(&params);
	tmp = params.philosophers;
	while (tmp)
	{
		pthread_create(tmp->thread, NULL, &simulation, tmp);
		tmp = tmp->next;
	}
	wait_philosophers(&params);
	usleep(100000);
//	clean_all(&params);
}
