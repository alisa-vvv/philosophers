// ************************************************************************** //
//                                                                            //
//                                                       ::::::::             //
//   setup_variables.c                                 :+:    :+:             //
//                                                    +:+                     //
//   By: avaliull <avaliull@student.codam.nl>        +#+                      //
//                                                  +#+                       //
//   Created: 2025/09/15 19:50:28 by avaliull     #+#    #+#                  //
//   Updated: 2025/10/07 17:34:41 by avaliull     ########   odam.nl          //
//                                                                            //
// ************************************************************************** //

#include "philo.h"
#include <stdio.h>

int	instantiate_subjects_and_objects(
	t_philo_args philo_args,
	t_philo *const philosophers,
	t_forkex *const forkexes
)
{
	int	i;
	int	err_check;

	i = -1;
	while (++i < philo_args.philo_count)
	{
		forkexes[i].fork = -1;
		if (pthread_mutex_init(&forkexes[i].mutex, NULL) != 0)
			return (1);
		philosophers[i] = -1;
	}
	return (0);
}

t_philo_errno	set_philo_args(
	t_philo_args *const args,
	const char *const *const argv
)
{
	args->philo_count = philo_atoi((unsigned char *) argv[1]);
	args->time_to_die = philo_atoi((unsigned char *) argv[2]);
	args->time_to_eat = philo_atoi((unsigned char *) argv[3]);
	args->time_to_sleep = philo_atoi((unsigned char *) argv[4]);
	if (args->time_to_die <= 0 || args->time_to_eat <= 0
		|| args->time_to_sleep <= 0)
	 	return (invalid_argument);
	args->time_to_eat *= 1000;
	args->time_to_sleep *= 1000;
	if (argv[5])
	{
		args->meal_count = philo_atoi((unsigned char *) argv[5]);
		if (args->meal_count <= 0)
	 		return (invalid_argument);
	}
	else
		args->meal_count = NO_LIMIT;
	return (success);
}
