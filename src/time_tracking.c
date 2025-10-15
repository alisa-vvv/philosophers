// ************************************************************************** //
//                                                                            //
//                                                       ::::::::             //
//   time_tracking.c                                   :+:    :+:             //
//                                                    +:+                     //
//   By: avaliull <avaliull@student.codam.nl>        +#+                      //
//                                                  +#+                       //
//   Created: 2025/09/15 19:39:18 by avaliull     #+#    #+#                  //
//   Updated: 2025/10/07 16:25:35 by avaliull     ########   odam.nl          //
//                                                                            //
// ************************************************************************** //

#include <stdio.h>

#include <sys/time.h>
#include <stddef.h>
#include "philo.h"

//MSG_TYPE += 0
//TIMESTAMP += 1
//PHILO += 2

int	log_action(
	t_thread_data *episteme,
	int philo_i,
	t_msg_type msg_type,
	unsigned long timestamp
)
{
	int	msg_index;

	pthread_mutex_lock(episteme->log_mutex);
	msg_index = *episteme->log_index;
	//printf("current index: %d\n", msg_index);
	if (*episteme->log_index == LOG_BUF_MAX - 3)
		*episteme->log_index = 0;
	else
		*episteme->log_index = *episteme->log_index + 3;
	if (episteme->log_arr[msg_index] != 0)
	{
		pthread_mutex_unlock(episteme->log_mutex);
		//printf("PLACEHOLDER, BUFFER OVERFLOW\n");
		return (1);
	}
	episteme->log_arr[msg_index] = msg_type;
	episteme->log_arr[msg_index + 1] = timestamp;
	episteme->log_arr[msg_index + 2] = philo_i;
	pthread_mutex_unlock(episteme->log_mutex);
	return (0); // where return?
}

unsigned long	get_timestamp_in_ms(
	unsigned long start_timestamp
)
{
	struct timeval	current_time;
	unsigned long	new_timestamp;

	gettimeofday(&current_time, NULL);
	new_timestamp = current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
	new_timestamp -= start_timestamp;
	return (new_timestamp);
}

unsigned long	get_start_timestamp(
	void
)
{
	struct timeval	timestamp;
	unsigned long	start_timestamp;

	gettimeofday(&timestamp, NULL);
	start_timestamp = timestamp.tv_sec * 1000 + timestamp.tv_usec / 1000;
	return (start_timestamp);
}
