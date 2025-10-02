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
	t_panopticon_data *const panopticon_data,
	char *msg_buffer,
	int *msg_buf_i,
	unsigned long *loop_stamp
)
{
	if (msg_buffer[0] != '\0')
	{
		write(STDOUT_FILENO, msg_buffer, *msg_buf_i + 1);
		memset(msg_buffer, 0, *msg_buf_i + 1);
		*msg_buf_i = 0;
	}
	*loop_stamp = get_timestamp_in_ms(panopticon_data->start_timestamp);
	return (NULL);
}

void	prepare_panopticon(
	t_panopticon_data *const panopticon_data,
	char *msg_buffer
)
{
	memset(msg_buffer, 0, MSG_BUF_MAX);
	pthread_mutex_lock(panopticon_data->start->mutex);
	if (panopticon_data->start->run_simulation == true)
		panopticon_data->start_timestamp = panopticon_data->start->timestamp;
	pthread_mutex_unlock(panopticon_data->start->mutex);
}

// stupid
//int	find_last_log_record(
//)
//{
//	if (msg_buf_i == 0)
//		loop_stamp = get_timestamp_in_ms(panopticon_data->start_timestamp);
//	usleep(1000);
//	pthread_mutex_lock(panopticon_data->log_mutex);
//	msg_info_local.log_index = *panopticon_data->log_index;
//	pthread_mutex_unlock(panopticon_data->log_mutex);
//	if (msg_info_local.log_index < i)
//		goal = LOG_BUF_MAX;
//	else
//		goal = msg_info_local.log_index;
//	return (goal);
//}

int	logger_loop(
	t_panopticon_data *const panopticon_data,
	char *msg_buffer,
	int *msg_buf_i,
	int	*i,
	unsigned long *loop_stamp
)
{
	int					goal;
	t_msg_info_local	msg_info_local;

	pthread_mutex_lock(panopticon_data->log_mutex);
	msg_info_local.log_index = *panopticon_data->log_index;
	pthread_mutex_unlock(panopticon_data->log_mutex);
	if (msg_info_local.log_index < *i)
		goal = LOG_BUF_MAX;
	else
		goal = msg_info_local.log_index;
	while (*i < goal)
	{
		assert(msg_buffer[*msg_buf_i] == 0); // REMOVE
		get_log_values(panopticon_data, &msg_info_local, *i);
		if (log_to_str(panopticon_data, &msg_info_local, msg_buffer, msg_buf_i) != 0)
			return (1);
		else if (get_timestamp_in_ms(panopticon_data->start_timestamp) - *loop_stamp > 25
				|| *msg_buf_i > (MSG_BUF_MAX / 4) * 3)
			write_and_clear_msg_buf(panopticon_data, msg_buffer, msg_buf_i, loop_stamp);
		if (panopticon_data->philos_sated == panopticon_data->philo_count)
		{
			write_and_clear_msg_buf(panopticon_data, msg_buffer, msg_buf_i, loop_stamp);
			return (1);
		}
		if (*i == LOG_BUF_MAX - 3)
		{
			if (goal == LOG_BUF_MAX)
				goal = msg_info_local.log_index;
			*i = 0;
		}
		else
			*i = *i + 3;
	}
	return (0);
}

void	log_and_write(
	t_panopticon_data	*const panopticon_data,
	char				*msg_buffer
)
{
	int				msg_buf_i;
	int				i;
	unsigned long	loop_stamp;
	int				loop_check;

	msg_buf_i = 0;
	i = 0;
	loop_stamp = get_timestamp_in_ms(panopticon_data->start_timestamp);
	while (1)
	{
		usleep(1000);
		loop_check = logger_loop(panopticon_data, msg_buffer, &msg_buf_i, &i, &loop_stamp);
		if (loop_check != 0)
			return ;
	}
}

void	*panopticon(
	void *data
)
{
	t_panopticon_data	*const panopticon_data = (t_panopticon_data *) data;
	char				msg_buffer[MSG_BUF_MAX];
	int					loop_check;

	prepare_panopticon(panopticon_data, msg_buffer);
	log_and_write(panopticon_data, msg_buffer);
	return (NULL);
}
