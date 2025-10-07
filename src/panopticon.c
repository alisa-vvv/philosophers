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

static void	get_log_values(
	t_panopticon_data *const panopticon_data,
	t_msg_info *const local_info,
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

static void	write_and_clear_msg_buf(
	t_panopticon_data *const panopticon_data,
	t_msg_buf *msg_buf,
	unsigned long *loop_stamp
)
{
	if (msg_buf->arr[0] != '\0')
	{
		write(STDOUT_FILENO, msg_buf->arr, msg_buf->i + 1);
		memset(msg_buf->arr, 0, msg_buf->i + 1);
		msg_buf->i = 0;
	}
	*loop_stamp = get_timestamp_in_ms(panopticon_data->start_timestamp);
}

static int	find_last_log(
	t_panopticon_data *const panopticon_data,
	t_msg_info *msg_info,
	int *i
)
{
	int	goal;

	pthread_mutex_lock(panopticon_data->log_mutex);
	msg_info->log_index = *panopticon_data->log_index;
	pthread_mutex_unlock(panopticon_data->log_mutex);
	if (msg_info->log_index < *i)
		goal = LOG_BUF_MAX;
	else
		goal = msg_info->log_index;
	return (goal);
}

static void	adjust_index(
	int log_index,
	int *goal,
	int *i
)
{
	if (*i == LOG_BUF_MAX - 3)
	{
		if (*goal == LOG_BUF_MAX)
			*goal = log_index;
		*i = 0;
	}
	else
		*i = *i + 3;
}

int	logger_loop(
	t_panopticon_data *const panopticon_data,
	t_msg_buf *msg_buf,
	int	*i,
	unsigned long *loop_stamp
)
{
	int			goal;
	t_msg_info	msg_info;

	goal = find_last_log(panopticon_data, &msg_info, i);
	while (*i < goal)
	{
		assert(msg_buf->arr[msg_buf->i] == 0); // REMOVE
		get_log_values(panopticon_data, &msg_info, *i);
		if (log_to_str(panopticon_data, &msg_info, msg_buf) != 0)
			return (1);
		else if (get_timestamp_in_ms(panopticon_data->start_timestamp) - *loop_stamp > 25
				|| msg_buf->i > (MSG_BUF_MAX / 4) * 3)
			write_and_clear_msg_buf(panopticon_data, msg_buf, loop_stamp);
		if (panopticon_data->philos_sated == panopticon_data->philo_count)
		{
			write_and_clear_msg_buf(panopticon_data, msg_buf, loop_stamp);
			return (1);
		}
		adjust_index(msg_info.log_index, &goal, i);
	}
	return (0);
}

void	log_and_write(
	t_panopticon_data	*const panopticon_data,
	t_msg_buf *msg_buf
)
{
	int				i;
	unsigned long	loop_stamp;
	int				loop_more;

	msg_buf->i = 0;
	i = 0;
	loop_stamp = get_timestamp_in_ms(panopticon_data->start_timestamp);
	while (1)
	{
		usleep(1000);
		loop_more = logger_loop(panopticon_data, msg_buf, &i, &loop_stamp);
		if (loop_more != 0)
			return ;
	}
}

void	*panopticon(
	void *data
)
{
	t_panopticon_data	*const panopticon_data = (t_panopticon_data *) data;
	t_msg_buf			msg_buf;
	int					loop_check;

	memset(msg_buf.arr, 0, MSG_BUF_MAX);
	msg_buf.i = 0;
	pthread_mutex_lock(panopticon_data->start->mutex);
	if (panopticon_data->start->run_simulation == true)
		panopticon_data->start_timestamp = panopticon_data->start->timestamp;
	pthread_mutex_unlock(panopticon_data->start->mutex);
	log_and_write(panopticon_data, &msg_buf);
	return (NULL);
}
