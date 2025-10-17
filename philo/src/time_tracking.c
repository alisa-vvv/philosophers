/* ************************************************************************** */
/*                                                                            */
/*                                                            ::::::::        */
/*   time_tracking.c                                         :+:    :+:       */
/*                                                          +:+               */
/*   By: avaliull <avaliull@student.codam.nl>              +#+                */
/*                                                        +#+                 */
/*   Created: 2025/10/17 13:28:04 by avaliull            #+#    #+#           */
/*   Updated: 2025/10/17 13:28:06 by avaliull            ########   odam.nl   */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include <stddef.h>
#include "philo.h"

void	log_action(
	t_thread_data *episteme,
	int philo_i,
	t_msg_type msg_type,
	unsigned long timestamp
)
{
	int	msg_index;

	pthread_mutex_lock(episteme->log_mutex);
	msg_index = *episteme->log_index;
	if (msg_index < 0)
	{
		pthread_mutex_unlock(episteme->log_mutex);
		return ;
	}
	if (*episteme->log_index == LOG_BUF_MAX - 1)
		*episteme->log_index = 0;
	else
		(*episteme->log_index)++;
	if (episteme->log[msg_index].msg_type != 0
		|| episteme->log[msg_index].timestamp != 0
		|| episteme->log[msg_index].philo_i != 0)
		*episteme->log_index = -1;
	else
	{
		episteme->log[msg_index].msg_type = msg_type;
		episteme->log[msg_index].timestamp = timestamp;
		episteme->log[msg_index].philo_i = philo_i;
	}
	pthread_mutex_unlock(episteme->log_mutex);
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

unsigned long	get_start_timestamp(void)
{
	struct timeval	timestamp;
	unsigned long	start_timestamp;

	gettimeofday(&timestamp, NULL);
	start_timestamp = timestamp.tv_sec * 1000 + timestamp.tv_usec / 1000;
	return (start_timestamp);
}
