#include "philosophers.h"

static int	ft_is_minus(char c)
{
	if (c == '-')
		return (-1);
	return (1);
}

static int	helper(int minus)
{
	if (minus == -1)
		return (0);
	else
		return (-1);
}

int	ft_atoi(const char *str)
{
	int				minus;
	long long int	output;
	int				i;

	i = 0;
	minus = 1;
	output = 0;
	if (!*str)
		return (0);
	if (str[i] == '-')
		minus = ft_is_minus(str[i++]);
	else if (str[i] == '+')
		i++;
	while (str[i])
	{
		if (str[i] <= '9' && str[i] >= '0')
		{
			output = output * 10 + (str[i++] - 48);
			if (i >= 19)
				return (helper(minus));
		}
		else
			return (output * minus);
	}
	return (minus * output);
}

static void	ft_is_digit(char * s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] < 48 || s[i] > 57)
			{
				printf("Error: Wrong argument format\n");
				exit (-1);
			};
		i++;
	}
}

void ft_init_params(t_params *params, int argc, char *argv[])
{
	ft_is_digit(argv[1]);
	params->times = malloc(sizeof(t_times));
	params->times->philo_number = ft_atoi(argv[1]);
	ft_is_digit(argv[2]);
	params->times->die_time = ft_atoi(argv[2]);
	ft_is_digit(argv[3]);
	params->times->eat_time = ft_atoi(argv[3]);
	ft_is_digit(argv[4]);
	params->times->sleep_time = ft_atoi(argv[4]);
	if (argc == 6)
	{
		ft_is_digit(argv[5]);
		params->times->meal_number = ft_atoi(argv[5]);
	}
	else
		params->times->meal_number = -1;
	params->philosophers = malloc(sizeof(t_philosopher));
	 if (params->times->philo_number < 2 || params->times->meal_number   == 0
	 	|| !(params->philosophers))
	 {
	 	printf("Error: Wrong arguments. the simulation won't work\n");
	 	exit(0);
	 }
}
