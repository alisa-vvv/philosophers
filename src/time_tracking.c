// ************************************************************************** //
//                                                                            //
//                                                       ::::::::             //
//   time_tracking.c                                   :+:    :+:             //
//                                                    +:+                     //
//   By: avaliull <avaliull@student.codam.nl>        +#+                      //
//                                                  +#+                       //
//   Created: 2025/09/15 19:39:18 by avaliull     #+#    #+#                  //
//   Updated: 2025/09/15 19:40:58 by avaliull     ########   odam.nl          //
//                                                                            //
// ************************************************************************** //

#include <assert.h>
#include <stdio.h>

#include <sys/time.h>
#include <stddef.h>
#include "philo.h"

void	log_action(
	t_thread_data *episteme,
	int philo_index,
	t_msg_type msg_type,
	unsigned long timestamp
)
{
	int	msg_index;

	//MSG_TYPE += 0
	//TIMESTAMP += 1
	//PHILO += 2
	if (check_simulation_end(episteme) == 1 && msg_type != MSG_DEAD)
	{
		printf("we ger here?\n");
		return ;
	}
	pthread_mutex_lock(episteme->log_mutex);
	//printf("*episteme->log_index: %lu\n", *episteme->log_index);
	msg_index = *episteme->log_index;
	if (*episteme->log_index == LOG_BUF_MAX - 3)
		*episteme->log_index = 0;
	else
		*episteme->log_index = *episteme->log_index + 3;
	//printf("msg_info->first_free_index after log: %d\n", msg_info->first_free_index);
	//pthread_mutex_unlock(&msg_info->first_free_index_mutex);
	//pthread_mutex_lock(&msg_info->msg_type_mutex);
	assert(episteme->log_arr[msg_index] == 0); // REMOVE
	episteme->log_arr[msg_index] = msg_type;
	//pthread_mutex_unlock(&msg_info->msg_type_mutex);
	//pthread_mutex_lock(&msg_info->timestamp_mutex);
	episteme->log_arr[msg_index + 1] = timestamp;
	//pthread_mutex_unlock(&msg_info->timestamp_mutex);
	//pthread_mutex_lock(&msg_info->philo_index_mutex);
	episteme->log_arr[msg_index + 2] = philo_index;
	//pthread_mutex_unlock(&msg_info->philo_index_mutex);
	pthread_mutex_unlock(episteme->log_mutex);
}

unsigned long	get_timestamp_in_ms(
	unsigned long	start_timestamp
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

