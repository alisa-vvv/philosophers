// ************************************************************************** //
//                                                                            //
//                                                       ::::::::             //
//   philo_states.c                                    :+:    :+:             //
//                                                    +:+                     //
//   By: avaliull <avaliull@student.codam.nl>        +#+                      //
//                                                  +#+                       //
//   Created: 2025/09/15 19:42:24 by avaliull     #+#    #+#                  //
//   Updated: 2025/10/01 15:37:42 by avaliull     ########   odam.nl          //
//                                                                            //
// ************************************************************************** //

#include "philo.h"
#include <pthread.h>
#include <sys/time.h>

void	philo_think(
	t_thread_data *episteme
)
{
	unsigned long	timestamp;

	if (*episteme->philo != THINKING)	
	{
		*episteme->philo = THINKING;
		timestamp = get_timestamp_in_ms(episteme->start_timestamp);
		log_action(episteme, episteme->philo_i, MSG_THINK, timestamp);
	}
}

//optimize all of this
		#include <stdio.h> // kfldsfjkdfjksd
int	philo_sleep(
	t_thread_data *episteme,
	unsigned long *last_eaten,
	int philo_i
)
{
	const unsigned long	start_timestamp = episteme->start_timestamp;
	const unsigned long	time_to_sleep_in_ms = episteme->time_to_sleep / 1000;
	unsigned long	sleep_start;
	unsigned long	time_slept;

	*episteme->philo = SLEEPING;
	sleep_start = get_timestamp_in_ms(start_timestamp);
	log_action(episteme, episteme->philo_i, MSG_SLEEP, sleep_start);
	time_slept = get_timestamp_in_ms(start_timestamp) - sleep_start;
	while (time_slept < time_to_sleep_in_ms)
	{
		usleep(5000);
		if (check_if_dead(episteme, last_eaten) == 1)
			return (1);
		time_slept = get_timestamp_in_ms(start_timestamp) - sleep_start;
	}
	return (0);
}

// conversions need to be changed all around to avoid useless calculations
// so time_to_eat_in_ms probably not needed
// add checks for end of simulation?
int	philo_eat(
	t_thread_data *episteme,
	unsigned long *last_eaten,
	int	philo_i,
	int *forks_held
)
{
	const unsigned long	start_timestamp = episteme->start_timestamp;
	const unsigned long	time_to_eat_in_ms = episteme->time_to_eat / 1000;
	unsigned long		new_timestamp;
	unsigned long		time_eaten;

	*last_eaten = get_timestamp_in_ms(episteme->start_timestamp);
	log_action(episteme, episteme->philo_i, MSG_EAT, *last_eaten);
	*episteme->philo = EATING;
	time_eaten = 0;
	while (time_eaten < time_to_eat_in_ms)
	{
		usleep(5000);
		if (time_eaten > episteme->time_to_die)
			return (1);
		time_eaten = get_timestamp_in_ms(start_timestamp) - *last_eaten;
	}
	pthread_mutex_lock(&episteme->right_forkex->mutex);
	episteme->right_forkex->fork = UNUSED;
	pthread_mutex_unlock(&episteme->right_forkex->mutex);
	pthread_mutex_lock(&episteme->left_forkex->mutex);
	episteme->left_forkex->fork = UNUSED;
	pthread_mutex_unlock(&episteme->left_forkex->mutex);
	*forks_held = 0;
	return (0);
}

