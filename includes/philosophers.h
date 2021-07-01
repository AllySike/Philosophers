/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgale <kgale@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/24 03:42:11 by lmartin           #+#    #+#             */
/*   Updated: 2021/07/01 21:37:14 by kgale            ###   ########.fr       */
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
	pthread_mutex_t	*fork_mutex;
	int				last_numb;
}				t_fork;

typedef struct		s_philosopher
{
	int				number;
	pthread_t		*thread;
	t_fork			*left_fork;
	t_fork			*right_fork;
	struct timeval	*last_meal_time;
	pthread_mutex_t	*last_meal_mutex;
	int				meal_count;
	void			*next;
}					t_philosopher;

typedef struct		t_params
{
	int				die_time;
	int				eat_time;
	int				sleep_time;
	int				eat_number;
	int				philo_number;
	struct timeval	*start_time;
	t_philosopher	*philosophers;
	t_philosopher	*curr;
}					t_params;


void	ft_init_params(t_params *params, int argc, char *argv[]);
void	ft_init_philosophers(t_params *params);
void	*simulation(void *args);
int		ft_atoi(const char *str);

#endif
