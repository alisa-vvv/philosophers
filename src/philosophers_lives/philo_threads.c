// ************************************************************************** //
//                                                                            //
//                                                       ::::::::             //
//   philo_threads.c                                   :+:    :+:             //
//                                                    +:+                     //
//   By: avaliull <avaliull@student.codam.nl>        +#+                      //
//                                                  +#+                       //
//   Created: 2025/10/08 14:51:27 by avaliull     #+#    #+#                  //
//   Updated: 2025/10/08 17:31:00 by avaliull     ########   odam.nl          //
//                                                                            //
// ************************************************************************** //

#include "philo.h"
#include <pthread.h>

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
