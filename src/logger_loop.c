// ************************************************************************** //
//                                                                            //
//                                                       ::::::::             //
//   logger_loop.c                                     :+:    :+:             //
//                                                    +:+                     //
//   By: avaliull <avaliull@student.codam.nl>        +#+                      //
//                                                  +#+                       //
//   Created: 2025/10/07 15:57:18 by avaliull     #+#    #+#                  //
//   Updated: 2025/10/07 15:58:03 by avaliull     ########   odam.nl          //
//                                                                            //
// ************************************************************************** //

#include "philo.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>

static void	get_log_values(
	t_panopticon_data *const panopticon_data,
	t_msg_info *const local_info,
	int i
)
{
	pthread_mutex_lock(panopticon_data->log_mutex);
	if (i > LOG_BUF_MAX - 3)
		printf("what the fuck, i: %d\n", i); // what
	local_info->msg_type = panopticon_data->log_arr[i];
	panopticon_data->log_arr[i] = 0;
	local_info->timestamp = panopticon_data->log_arr[i + 1];
	panopticon_data->log_arr[i + 1] = 0;
	local_info->philo_i = panopticon_data->log_arr[i + 2];
	panopticon_data->log_arr[i + 2] = 0;
	pthread_mutex_unlock(panopticon_data->log_mutex);
}

static void	print_and_clear_msg_buf(
	t_panopticon_data *const panopticon_data,
	t_msg_buf *msg_buf,
	unsigned long *loop_stamp
)
{
	if (msg_buf->arr[0] != '\0')
	{
		printf("%s", msg_buf->arr);
		memset(msg_buf->arr, 0, msg_buf->i + 1);
		msg_buf->i = 0;
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
	assert(panopticon_data->log_arr[msg_info->log_index] == 0);
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

//static int	count_meals(
//	t_panopticon_data *const panopticon_data,
//	const int philo_i
//)
//{
//	panopticon_data->meals_eaten[philo_i]++;
//	//printf("meals eaten after: %d\n", panopticon_data->meals_eaten[philo_i]);
//	if (panopticon_data->meals_eaten[philo_i] == panopticon_data->meal_count)
//	{
//		//printf("panopticon_data->philos_sated before: %d\n", panopticon_data->philos_sated);
//		panopticon_data->philos_sated++;
//		//printf("panopticon_data->philos_sated after: %d\n", panopticon_data->philos_sated);
//	}
//	if (panopticon_data->philos_sated == panopticon_data->philo_count)
//	{
//		pthread_mutex_lock(panopticon_data->start->mutex);
//		panopticon_data->start->run_simulation = false;
//		pthread_mutex_unlock(panopticon_data->start->mutex);
//		return (1);
//	}
//	return (0);
//}
//
//int	print_msg(
//	t_panopticon_data *panopticon_data,
//	t_msg_info msg_info
//)
//{
//	if (msg_info.msg_type == MSG_THINK)
//		printf("%lu philosopher %d is thinking\n",
//			msg_info.timestamp, msg_info.philo_i + 1);
//	else if (msg_info.msg_type == MSG_FORK)
//		printf("%lu philosopher %d took a fork\n",
//			msg_info.timestamp, msg_info.philo_i + 1);
//	else if (msg_info.msg_type == MSG_EAT)
//	{
//		printf("%lu philosopher %d is eating\n",
//			msg_info.timestamp, msg_info.philo_i + 1);
//		return (count_meals(panopticon_data, msg_info.philo_i));
//	}
//	else if (msg_info.msg_type == MSG_SLEEP)
//		printf("%lu philosopher %d is sleeping\n",
//			msg_info.timestamp, msg_info.philo_i + 1);
//	else if (msg_info.msg_type == MSG_DEAD)
//	{
//		printf("%lu philosopher %d died\n",
//			msg_info.timestamp, msg_info.philo_i + 1);
//		return (1);
//	}
//	return (0);
//}
//
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
	while (*i != goal)
	{
		if (msg_buf->i > (MSG_BUF_MAX / 4) * 3)
			print_and_clear_msg_buf(panopticon_data, msg_buf, loop_stamp);
		get_log_values(panopticon_data, &msg_info, *i);
		adjust_index(msg_info.log_index, &goal, i);
		if (log_to_str(panopticon_data, &msg_info, msg_buf) == dead
			|| panopticon_data->philos_sated == panopticon_data->philo_count)
		{
			printf("%s", msg_buf->arr);
			pthread_mutex_lock(panopticon_data->start->mutex);
			panopticon_data->start->run_simulation = false;
			pthread_mutex_unlock(panopticon_data->start->mutex);
			return (1);
		}
	}
	print_and_clear_msg_buf(panopticon_data, msg_buf, loop_stamp);
	return (0);
}
