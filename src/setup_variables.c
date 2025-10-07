// ************************************************************************** //
//                                                                            //
//                                                       ::::::::             //
//   setup_variables.c                                 :+:    :+:             //
//                                                    +:+                     //
//   By: avaliull <avaliull@student.codam.nl>        +#+                      //
//                                                  +#+                       //
//   Created: 2025/09/15 19:50:28 by avaliull     #+#    #+#                  //
//   Updated: 2025/10/07 15:59:49 by avaliull     ########   odam.nl          //
//                                                                            //
// ************************************************************************** //

#include "philo.h"
#include <stdio.h>

int	construct_paradigm(
	t_thread_data *episteme,
	pthread_mutex_t *log_mutex,
	unsigned long *log_index,
	unsigned long *log_arr,
	t_philo *philosophers,
	t_philo_args philo_args,
	t_forkex *forkexes,
	t_start *start
)
{
	int	i;

	i = -1;
	while (++i < philo_args.philo_count)
	{
		episteme[i].start = start;
		episteme[i].log_index = log_index;
		episteme[i].log_mutex = log_mutex;
		episteme[i].log_arr = log_arr;
		episteme[i].philo = &philosophers[i];
		episteme[i].philo_count = philo_args.philo_count;
		episteme[i].time_to_die = philo_args.time_to_die;
		episteme[i].time_to_eat = philo_args.time_to_eat;
		episteme[i].time_to_sleep = philo_args.time_to_sleep;
		episteme[i].meal_count = philo_args.meal_count;
		episteme[i].left_forkex = &forkexes[i];
		if (i != philo_args.philo_count - 1)
			episteme[i].right_forkex = &forkexes[i + 1];
		else
			episteme[i].right_forkex = forkexes;
		episteme[i].philo_i = i;
	}
	return (0);
}

int	instantiate_subjects_and_objects(
	t_philo_args philo_args,
	t_philo *philosophers,
	t_forkex *forkexes
)
{
	int	i;

	i = -1;
	while (++i < philo_args.philo_count)
	{
		forkexes[i].fork = -1;
		pthread_mutex_init(&forkexes[i].mutex, NULL);
		philosophers[i] = -1;
	}
	return (0);
}

t_philo_errno	set_philo_args(
	t_philo_args *args,
	char **argv
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
