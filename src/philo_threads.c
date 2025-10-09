// ************************************************************************** //
//                                                                            //
//                                                       ::::::::             //
//   philo_threads.c                                   :+:    :+:             //
//                                                    +:+                     //
//   By: avaliull <avaliull@student.codam.nl>        +#+                      //
//                                                  +#+                       //
//   Created: 2025/10/08 14:51:27 by avaliull     #+#    #+#                  //
//   Updated: 2025/10/08 17:14:04 by avaliull     ########   odam.nl          //
//                                                                            //
// ************************************************************************** //

#include "philo.h"
#include <pthread.h>

static int	routine(
	t_thread_data *const episteme,
	unsigned long *const last_eaten,
	int *const times_eaten,
	int *const forks_held
)
{
	int				stop;
	unsigned long	timestamp;

	philo_think(episteme);
	find_free_forks(episteme, forks_held);
	if (check_if_dead(episteme, last_eaten) == 1)
		return (1);
	if (*forks_held == 2)
	{
		*episteme->philo = EATING;
		stop = philo_eat(episteme, last_eaten, episteme->philo_i, forks_held);
		if (stop == 1)
			return (1);
		if (episteme->meal_count != NO_LIMIT)
			(*times_eaten)++;
		stop = philo_sleep(episteme, last_eaten, episteme->philo_i);
		if (stop == 1)
			return (1);
	}
	if (check_if_dead(episteme, last_eaten) == 1)
		return (1);
	return (0);
}

static int	check_simulation_end(
	t_thread_data	*const episteme
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
	t_thread_data *const	episteme = (t_thread_data *) data;
	unsigned long			last_eaten;
	int						times_eaten;
	int						total_meals;
	int						forks_held;

	last_eaten = 0;
	times_eaten = 0;
	forks_held = 0;
	pthread_mutex_lock(episteme->start->mutex);
	if (episteme->start->run_simulation == false) // in case of error during setup
		return (NULL);
	episteme->start_stamp = episteme->start->timestamp;
	pthread_mutex_unlock(episteme->start->mutex);
	if (episteme->meal_count >= 0)
		total_meals = episteme->meal_count;
	else if (episteme->meal_count == NO_LIMIT)
		total_meals = 1;
	while (times_eaten < total_meals)
	{
		if (check_simulation_end(episteme) == 1)
			return (NULL);
		if (routine(episteme, &last_eaten, &times_eaten, &forks_held) == 1)
			return (NULL);
	}
	return (NULL);
}

static int	try_join(
	pthread_t *const thread,
	t_start *const start
)
{
	if (pthread_join(*thread, NULL) != 0)
	{
		if (pthread_mutex_lock(start->mutex) != 0)
			return (thread_join_fail);
		start->run_simulation = false;
		pthread_mutex_unlock(start->mutex);
		return (thread_join_fail);
	}
	return (0);
}

static int	join_threads(
	pthread_t *const panopticon_thread,
	pthread_t *const philo_threads,
	t_start *const start,
	const t_philo_args philo_args
)
{
	int	err;
	int	i;

	if (pthread_mutex_unlock(start->mutex) != 0)
	{
		start->run_simulation = false;
		return (mutex_unlock_fail);
	}
	err = try_join(panopticon_thread, start);
	if (err != 0)
		return (err);
	i = -1;
	while (++i < philo_args.philo_count)
	{
		err = try_join(&philo_threads[i], start);
		if (err != 0)
			return (err);
	}
	return (0);
}

static int	create_philo_threads(
	const t_philo_args philo_args,
	pthread_t *philo_threads,
	t_thread_data *const episteme,
	t_start *const start
)
{
	int	i;

	i = -1;
	while (++i < philo_args.philo_count)
	{
		if (pthread_create(&philo_threads[i], NULL, praxis, &episteme[i]) != 0)
		{
			pthread_mutex_unlock(start->mutex);
			return (thread_create_fail);
		}
	}
	return (0);
}

int	run_threads(
	t_thread_data *const episteme,
	t_panopticon_data *const panopticon_data,
	const t_philo_args philo_args,
	t_start *const start
)
{
	pthread_t	panopticon_thread;
	pthread_t	philo_threads[PHILO_BUF_MAX];
	int			i;
	int			err;

	if (pthread_mutex_lock(start->mutex) != 0)
		return (mutex_lock_fail);
	if (pthread_create(&panopticon_thread, NULL, panopticon, panopticon_data)
		!= 0)
		return (thread_create_fail);
	if (create_philo_threads(philo_args, philo_threads, episteme, start) != 0)
	{
		pthread_mutex_unlock(start->mutex);
		return (thread_create_fail);
	}
	usleep(1000);
	start->run_simulation = true;
	start->timestamp = get_start_stamp();
	return (join_threads(&panopticon_thread, philo_threads, start, philo_args));
}
