// ************************************************************************** //
//                                                                            //
//                                                       ::::::::             //
//   panopticon.c                                      :+:    :+:             //
//                                                    +:+                     //
//   By: avaliull <avaliull@student.codam.nl>        +#+                      //
//                                                  +#+                       //
//   Created: 2025/09/19 16:26:03 by avaliull     #+#    #+#                  //
//   Updated: 2025/10/01 15:00:13 by avaliull     ########   odam.nl          //
//                                                                            //
// ************************************************************************** //

#include <assert.h>

#include "philo.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void	get_log_values(
	t_panopticon_data *const panopticon_data,
	t_msg_info_local *const local_info,
	int i
)
{
	pthread_mutex_lock(panopticon_data->log_mutex);
	local_info->msg_type = panopticon_data->log_arr[i];
	panopticon_data->log_arr[i] = 0;
	local_info->timestamp = panopticon_data->log_arr[i + 1];
	panopticon_data->log_arr[i + 1] = 0;
	local_info->philo_i = panopticon_data->log_arr[i + 2];
	panopticon_data->log_arr[i + 2] = 0;
	pthread_mutex_unlock(panopticon_data->log_mutex);
}

void	*write_and_clear_msg_buf(
	char *msg_buffer,
	int *msg_buf_i
)
{
	if (msg_buffer[0] != '\0')
	{
		write(STDOUT_FILENO, msg_buffer, *msg_buf_i + 1);
		memset(msg_buffer, 0, *msg_buf_i + 1);
		*msg_buf_i = 0;
	}
	return (NULL);
}

void	*panopticon(
	void *data
)
{
	t_panopticon_data	*const panopticon_data = (t_panopticon_data *) data;
	t_msg_info_local	msg_info_local;
	char				msg_buffer[MSG_BUF_MAX];
	int					msg_buf_i;
	int					goal;
	int					i;
	unsigned long		loop_stamp;

	memset(msg_buffer, 0, MSG_BUF_MAX);
	pthread_mutex_lock(panopticon_data->start->mutex);
	if (panopticon_data->start->run_simulation == true)
		panopticon_data->start_timestamp = panopticon_data->start->timestamp;
	pthread_mutex_unlock(panopticon_data->start->mutex);
	i = 0;
	msg_buf_i = 0;
	while (1)
	{
		if (msg_buf_i == 0)
			loop_stamp = get_timestamp_in_ms(panopticon_data->start_timestamp);
		usleep(1000);
		pthread_mutex_lock(panopticon_data->log_mutex);
		msg_info_local.log_index = *panopticon_data->log_index;
		pthread_mutex_unlock(panopticon_data->log_mutex);
		if (msg_info_local.log_index < i)
			goal = LOG_BUF_MAX;
		else
			goal = msg_info_local.log_index;
		while (i < goal)
		{
			assert(msg_buffer[msg_buf_i] == 0); // REMOVE
			get_log_values(panopticon_data, &msg_info_local, i);
			if (log_to_str(panopticon_data, &msg_info_local, msg_buffer, &msg_buf_i) != 0)
				return (NULL);
			else if (get_timestamp_in_ms(panopticon_data->start_timestamp) - loop_stamp > 25
					|| msg_buf_i > (MSG_BUF_MAX / 4) * 3)
				write_and_clear_msg_buf(msg_buffer, &msg_buf_i);
			if (panopticon_data->philos_sated == panopticon_data->philo_count)
				return (write_and_clear_msg_buf(msg_buffer, &msg_buf_i));
			if (i == LOG_BUF_MAX - 3)
			{
				if (goal == LOG_BUF_MAX)
					goal = msg_info_local.log_index;
				i = 0;
			}
			else
				i += 3;
		}
	}
	return (NULL);
}
