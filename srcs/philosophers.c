/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgale <kgale@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/16 18:58:31 by lmartin           #+#    #+#             */
/*   Updated: 2021/07/01 22:01:43 by kgale            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int		main(int argc, char *argv[])
{
	t_params		params;
	if (argc < 5 || argc > 6)
	{
		printf("Error: Wrong number of arguments\n");
		return (-1);
	}
	ft_init_params(&params, argc, argv);
	ft_init_philosophers(&params);
	//wait_philosophers
	///////////////////////
	// if (params.philosophers->right_fork->mutex == ((t_philosopher *)(params.philosophers->next))->left_fork->mutex )
		// printf("| %d |", ((t_philosopher *)(params.philosophers->next))->number);
}
