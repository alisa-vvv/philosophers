// ************************************************************************** //
//                                                                            //
//                                                       ::::::::             //
//   philo_states.c                                    :+:    :+:             //
//                                                    +:+                     //
//   By: avaliull <avaliull@student.codam.nl>        +#+                      //
//                                                  +#+                       //
//   Created: 2025/09/15 19:42:24 by avaliull     #+#    #+#                  //
//   Updated: 2025/09/15 19:42:25 by avaliull     ########   odam.nl          //
//                                                                            //
// ************************************************************************** //

#include "philo.h"
#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>

void	philo_think(
	t_thread_data *episteme
)
{
	unsigned long	timestamp;

	*episteme->philo = THINKING;
	timestamp = get_timestamp_in_ms(episteme->start_timestamp);
	log_action(episteme->philo_index, MSG_THINK, episteme->msg_info, timestamp);
}

//optimize all of this
void	philo_sleep(
	t_thread_data *episteme,
	int philo_index
)
{
	unsigned long	sleep_start;
	unsigned long	time_slept;
	const unsigned long	time_to_sleep_in_ms = episteme->philo_args.time_to_sleep / 1000;

	*episteme->philo = SLEEPING;
	sleep_start = get_timestamp_in_ms(episteme->start_timestamp);
	time_slept = get_timestamp_in_ms(episteme->start_timestamp) - sleep_start;
	log_action(philo_index, MSG_SLEEP, episteme->msg_info, sleep_start);
	usleep(episteme->philo_args.time_to_sleep / 2);
	//usleep(time_to_sleep_tenth * 9);
	while (time_slept < time_to_sleep_in_ms)
	{
		usleep(50);
		time_slept = get_timestamp_in_ms(episteme->start_timestamp) - sleep_start;
	}
}

// conversions need to be changed all around to avoid useless calculations
// so time_to_eat_in_ms probably not needed
void	philo_eat(
	t_thread_data *episteme,
	unsigned long *last_eaten,
	int	philo_index,
	int *forks_held
)
{
	const unsigned long	start_timestamp = episteme->start_timestamp;
	unsigned long		new_timestamp;
	unsigned long		time_eaten;
	const unsigned long	time_to_eat_in_ms = episteme->philo_args.time_to_eat / 1000;
	//const unsigned long	time_to_eat_tenth = episteme->philo_args.time_to_eat / 100;

	*last_eaten = get_timestamp_in_ms(episteme->start_timestamp);
	log_action(philo_index, MSG_EAT, episteme->msg_info, *last_eaten);
	*episteme->philo = EATING;
	time_eaten = 0;
	usleep(episteme->philo_args.time_to_eat / 2);
	//usleep(time_to_eat_tenth * 9);
	while (time_eaten < time_to_eat_in_ms)
	{
		usleep(50);
		time_eaten = get_timestamp_in_ms(start_timestamp) - *last_eaten;
	}
	// will changing up order here matter?
	// right fork is the one that the other philo is supposed to take first
	// maybe this will reduce some delays?
	pthread_mutex_lock(&episteme->right_forkex->mutex);
	episteme->right_forkex->fork = UNUSED;
	pthread_mutex_unlock(&episteme->right_forkex->mutex);
	pthread_mutex_lock(&episteme->left_forkex->mutex);
	episteme->left_forkex->fork = UNUSED;
	pthread_mutex_unlock(&episteme->left_forkex->mutex);
	*forks_held = 0;
	return ;
}

