// ************************************************************************** //
//                                                                            //
//                                                       ::::::::             //
//   philo_states.c                                    :+:    :+:             //
//                                                    +:+                     //
//   By: avaliull <avaliull@student.codam.nl>        +#+                      //
//                                                  +#+                       //
//   Created: 2025/09/15 19:42:24 by avaliull     #+#    #+#                  //
//   Updated: 2025/10/08 16:43:37 by avaliull     ########   odam.nl          //
//                                                                            //
// ************************************************************************** //

#include "philo.h"
#include <pthread.h>
#include <sys/time.h>

int	check_if_dead(
	t_thread_data *const episteme,
	unsigned long *const last_eaten
)
{
	unsigned long	timestamp;
	bool			do_log;

	timestamp = get_timestamp(episteme->start_stamp);
	if (timestamp - *last_eaten > episteme->time_to_die)
	{
		pthread_mutex_lock(episteme->start->mutex);
		if (episteme->start->run_simulation == false)
			do_log = false;
		else
			do_log = true;
		episteme->start->run_simulation = false;
		pthread_mutex_unlock(episteme->start->mutex);
		if (do_log == true)
			log_action(episteme, episteme->philo_i, MSG_DEAD, timestamp);
		return (1);
	}
	return (0);
}

void	philo_think(
	t_thread_data *const episteme
)
{
	unsigned long	timestamp;

	if (*episteme->philo != THINKING)	
	{
		*episteme->philo = THINKING;
		timestamp = get_timestamp(episteme->start_stamp);
		log_action(episteme, episteme->philo_i, MSG_THINK, timestamp);
	}
}

int	philo_sleep(
	t_thread_data *const episteme,
	unsigned long *const last_eaten,
	int philo_i
)
{
	const unsigned long	start_stamp = episteme->start_stamp;
	const unsigned long	time_to_sleep_in_ms = episteme->time_to_sleep / 1000;
	unsigned long	sleep_start;
	unsigned long	time_slept;

	*episteme->philo = SLEEPING;
	sleep_start = get_timestamp(start_stamp);
	log_action(episteme, episteme->philo_i, MSG_SLEEP, sleep_start);
	time_slept = get_timestamp(start_stamp) - sleep_start;
	while (time_slept < time_to_sleep_in_ms)
	{
		usleep(5000);
		if (check_if_dead(episteme, last_eaten) == 1)
			return (1);
		time_slept = get_timestamp(start_stamp) - sleep_start;
	}
	return (0);
}

static int put_down_forks(
	t_thread_data *const episteme,
	int *const forks_held
)
{
	if (pthread_mutex_lock(&episteme->right_forkex->mutex) != 0)
		return (1);
	episteme->right_forkex->fork = UNUSED;
	if (pthread_mutex_unlock(&episteme->right_forkex->mutex) != 0)
		return (1);
	if (pthread_mutex_lock(&episteme->left_forkex->mutex) != 0)
		return (1);
	episteme->left_forkex->fork = UNUSED;
	if (pthread_mutex_unlock(&episteme->left_forkex->mutex) != 0)
		return (1);
	*forks_held = 0;
	return (0);
}

int	philo_eat(
	t_thread_data *const episteme,
	unsigned long *const last_eaten,
	int	philo_i,
	int *const forks_held
)
{
	const unsigned long	start_stamp = episteme->start_stamp;
	const unsigned long	time_to_eat_in_ms = episteme->time_to_eat / 1000;
	unsigned long		new_timestamp;
	unsigned long		time_eaten;

	*last_eaten = get_timestamp(episteme->start_stamp);
	log_action(episteme, episteme->philo_i, MSG_EAT, *last_eaten);
	*episteme->philo = EATING;
	time_eaten = 0;
	while (time_eaten < time_to_eat_in_ms)
	{
		usleep(5000);
		if (time_eaten > episteme->time_to_die)
			return (1);
		time_eaten = get_timestamp(start_stamp) - *last_eaten;
	}
	put_down_forks(episteme, forks_held);
	return (0);
}
