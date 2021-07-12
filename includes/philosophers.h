/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgale <kgale@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/12 17:28:18 by kgale             #+#    #+#             */
/*   Updated: 2021/07/12 17:49:18 by kgale            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <stdlib.h>
# include <sys/stat.h>
# include <semaphore.h>
# include <sys/time.h>
# include <pthread.h>
# include <unistd.h>
# include <string.h>
# include <fcntl.h>
# include <stdio.h>

typedef struct s_fork
{
	pthread_mutex_t	*mutex;
	int				last_numb;
}				t_fork;

typedef struct s_times
{
	int				die_time;
	int				eat_time;
	int				sleep_time;
	int				meal_number;
	int				philo_number;
	struct timeval	*start_time;
}				t_times;

typedef struct s_philosopher
{
	int				number;
	pthread_t		*thread;
	t_fork			*left_fork;
	t_fork			*right_fork;
	struct timeval	*last_meal_time;
	t_times			*times;
	int				meal_count;
	void			*next;
}					t_philosopher;

typedef struct s_params
{
	t_times			*times;
	t_philosopher	*philosophers;
}					t_params;

void	*simulation(void *args);
void	ft_init_params(t_params *params, int argc, char *argv[]);
void	ft_init_philosophers(t_params *params);
void	ft_usleep(unsigned int n);
void	ft_error(t_params *params);
void	ft_exit(t_params *phi);
void	print_res(char *line, int number,
			struct timeval *t1, struct timeval *t2);
void	clean_philosopher(t_philosopher *phi);

#endif
