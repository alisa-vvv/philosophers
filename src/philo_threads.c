// ************************************************************************** //
//                                                                            //
//                                                       ::::::::             //
//   philo_threads.c                                   :+:    :+:             //
//                                                    +:+                     //
//   By: avaliull <avaliull@student.codam.nl>        +#+                      //
//                                                  +#+                       //
//   Created: 2025/10/07 16:15:37 by avaliull     #+#    #+#                  //
//   Updated: 2025/10/07 16:22:07 by avaliull     ########   odam.nl          //
//                                                                            //
// ************************************************************************** //

#include <stdio.h>

#include "philo.h"
#include <pthread.h>

static int	routine(
	t_thread_data *episteme,
	unsigned long *last_eaten,
	int *times_eaten,
	int *forks_held
)
{
	int				stop;
	unsigned long	timestamp;

	if (philo_think(episteme) == 1)
		return (1);
	if (find_free_forks(episteme, forks_held) == 1)
		return (1);
	if (check_if_dead(episteme, last_eaten) == 1)
		return (1);
	if (*forks_held == 2)
	{
		*episteme->philo = EATING;
		if (philo_eat(episteme, last_eaten, episteme->philo_i, forks_held) == 1)
			return (1);
		if (episteme->meal_count != NO_LIMIT)
			(*times_eaten)++;
		if (philo_sleep(episteme, last_eaten, episteme->philo_i) == 1)
			return (1);
	}	
	if (check_if_dead(episteme, last_eaten) == 1)
		return (1);
	return (0);
}

int	check_simulation_end(
	t_thread_data	*episteme
)
{
	pthread_mutex_lock(episteme->start->mutex);
	if (episteme->start->run_simulation == false)
	{
		pthread_mutex_unlock(episteme->start->mutex);
		return (1);
	}
	pthread_mutex_unlock(episteme->start->mutex);
	return (0);
}

static void	*praxis(
	void *data
)
{
	t_thread_data	*episteme = (t_thread_data *) data;
	unsigned long	last_eaten;
	int				times_eaten;
	int				total_meals;
	int				forks_held;

	last_eaten = 0;
	times_eaten = 0;
	forks_held = 0;
	pthread_mutex_lock(episteme->start->mutex);
	episteme->start_timestamp = episteme->start->timestamp;
	pthread_mutex_unlock(episteme->start->mutex);
	if (episteme->meal_count >= 0)
		total_meals = episteme->meal_count;
	else if (episteme->meal_count == NO_LIMIT)
		total_meals = 1;
	while (times_eaten < total_meals)
	{
		if (check_simulation_end(episteme) == 1)
			break ;
		if (routine(episteme, &last_eaten, &times_eaten, &forks_held) == 1)
			break ;
	}
	return (NULL);
}

int	run_threads(
	t_thread_data *episteme,
	t_panopticon_data *panopticon_data,
	t_philo_args philo_args,
	t_start *start
)
{
	pthread_t	philo_threads[PHILO_BUF_MAX];
	int			i;

	pthread_mutex_lock(start->mutex);
	start->run_simulation = true;
	start->timestamp = get_start_timestamp();
	panopticon_data->start_timestamp = start->timestamp;
	i = -1;
	while (++i < philo_args.philo_count)
		pthread_create(&philo_threads[i], NULL, praxis, &episteme[i]);
	pthread_mutex_unlock(start->mutex);
	panopticon(panopticon_data);
	i = -1;
	while (++i < philo_args.philo_count)
		pthread_join(philo_threads[i], NULL);
	return (0);
}
