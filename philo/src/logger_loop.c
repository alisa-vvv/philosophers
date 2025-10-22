/* ************************************************************************** */
/*                                                                            */
/*                                                            ::::::::        */
/*   logger_loop.c                                           :+:    :+:       */
/*                                                          +:+               */
/*   By: avaliull <avaliull@student.codam.nl>              +#+                */
/*                                                        +#+                 */
/*   Created: 2025/10/17 13:24:47 by avaliull            #+#    #+#           */
/*   Updated: 2025/10/17 13:31:03 by avaliull            ########   odam.nl   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

static void	get_log_values(
	t_panopticon_data *const panopticon_data,
	t_msg_info *const local_info,
	int i
)
{
	pthread_mutex_lock(panopticon_data->log_mutex);
	local_info->msg_type = panopticon_data->log[i].msg_type;
	panopticon_data->log[i].msg_type = 0;
	local_info->timestamp = panopticon_data->log[i].timestamp;
	panopticon_data->log[i].timestamp = 0;
	local_info->philo_i = panopticon_data->log[i].philo_i;
	panopticon_data->log[i].philo_i = 0;
	pthread_mutex_unlock(panopticon_data->log_mutex);
}

static void	print_and_clear_msg_buf(
	t_msg_buf *msg_buf
)
{
	if (msg_buf->arr[0] != '\0')
	{
		msg_buf->arr[msg_buf->i - 1] = '\0';
		printf("%s\n", msg_buf->arr);
		msg_buf->i = 0;
		msg_buf->arr[0] = '\0';
	}
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
	if (panopticon_data->log[msg_info->log_index].msg_type != 0)
	{
		philo_putstr_fd(STDERR_FILENO, "Error! Log overflow\n");
		pthread_mutex_unlock(panopticon_data->log_mutex);
		return (-1);
	}
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
	if (*i == LOG_BUF_MAX - 1)
	{
		if (*goal == LOG_BUF_MAX)
			*goal = log_index;
		*i = 0;
	}
	else
		(*i)++;
}

int	logger_loop(
	t_panopticon_data *const panopticon_data,
	t_msg_buf *msg_buf,
	int	*i
)
{
	int			goal;
	t_msg_info	msg_info;
	int			check_return;

	goal = find_last_log(panopticon_data, &msg_info, i);
	if (goal < 0)
		return (1);
	while (*i != goal)
	{
		if (msg_buf->i > (MSG_BUF_MAX / 4) * 3)
			print_and_clear_msg_buf(msg_buf);
		get_log_values(panopticon_data, &msg_info, *i);
		adjust_index(msg_info.log_index, &goal, i);
		check_return = log_to_str(panopticon_data, &msg_info, msg_buf);
		if (check_return == death)
			return (1);
		if (panopticon_data->philos_sated == panopticon_data->philo_count)
		{
			msg_buf->arr[msg_buf->i - 1] = '\0';
			return (1);
		}
		msg_buf->arr[msg_buf->i] = '\0';
	}
	print_and_clear_msg_buf(msg_buf);
	return (0);
}
