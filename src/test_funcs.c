/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   test_funcs.c                                      :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/05/30 17:57:50 by avaliull     #+#    #+#                  */
/*   Updated: 2025/05/30 17:58:11 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

void			test_print_args(
	t_philo_args *args
)
{
	printf("philo count: %lu\n", args->philo_count);
	printf("die time: %lu\n", args->time_to_die);
	printf("eat time: %lu\n", args->time_to_eat);
	printf("sleep time: %lu\n", args->time_to_sleep);
	printf("meal count: %d\n", args->meal_count);
}
