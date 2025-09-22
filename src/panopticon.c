// ************************************************************************** //
//                                                                            //
//                                                       ::::::::             //
//   panopticon.c                                      :+:    :+:             //
//                                                    +:+                     //
//   By: avaliull <avaliull@student.codam.nl>        +#+                      //
//                                                  +#+                       //
//   Created: 2025/09/19 16:26:03 by avaliull     #+#    #+#                  //
//   Updated: 2025/09/19 17:29:55 by avaliull     ########   odam.nl          //
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
	pthread_mutex_lock(&panopticon_data->msg_info->msg_type_mutex);
	local_info->msg_type = panopticon_data->msg_info->msg_type[i];
	panopticon_data->msg_info->msg_type[i] = 0;
	pthread_mutex_unlock(&panopticon_data->msg_info->msg_type_mutex);
	pthread_mutex_lock(&panopticon_data->msg_info->timestamp_mutex);
	local_info->timestamp = panopticon_data->msg_info->timestamp[i];
	panopticon_data->msg_info->timestamp[i] = 0;
	pthread_mutex_unlock(&panopticon_data->msg_info->timestamp_mutex);
	pthread_mutex_lock(&panopticon_data->msg_info->philo_index_mutex);
	local_info->philo_index = panopticon_data->msg_info->philo_index[i];
	panopticon_data->msg_info->philo_index[i] = 0;
	pthread_mutex_unlock(&panopticon_data->msg_info->philo_index_mutex);
}

int	philo_ultoa(
	unsigned long	var,
	char *str
)
{
	unsigned long	temp_var;
	int				var_index;
	int				number_len;

	temp_var = var;
	var_index = -1;
	while (temp_var != 0)
	{
		temp_var /= 10;
		var_index++;
	}
	number_len = var_index + 1;
	while (var_index >= 0)
	{
		str[var_index--] = (var % 10) + 48;
		var /= 10;
	}
	return (number_len);
}

void	philo_memcpy(
	char *src,
	char *dest,
	int count
)
{
	while (count--)
		dest[count] = src[count];
}

int	display_message(
	t_panopticon_data *panopticon_data,
	t_msg_info_local *msg_info,
	char *msg_buffer,
	int	*msg_buf_i
)
{
	*msg_buf_i += philo_ultoa(msg_info->timestamp, &msg_buffer[*msg_buf_i]);
	philo_memcpy(" philosopher ", &msg_buffer[*msg_buf_i], 13);
	*msg_buf_i = *msg_buf_i + 13;
	*msg_buf_i = *msg_buf_i + philo_ultoa(msg_info->philo_index + 1, &msg_buffer[*msg_buf_i]);
	if (msg_info->msg_type == MSG_DEAD)
	{
		philo_memcpy(" died\n", &msg_buffer[*msg_buf_i], 6);
		write(STDOUT_FILENO, msg_buffer, *msg_buf_i + 7);
		//printf("post msg: %lu\n", get_timestamp_in_ms(panopticon_data->start_timestamp));
		pthread_mutex_lock(panopticon_data->start->mutex);
		panopticon_data->start->run_simulation = false;
		pthread_mutex_unlock(panopticon_data->start->mutex);
		return (1);
	}	
	else if (msg_info->msg_type == MSG_THINK)
	{
		philo_memcpy(" is thinking\n", &msg_buffer[*msg_buf_i], 13);
		*msg_buf_i = *msg_buf_i + 13;
	}
	else if (msg_info->msg_type == MSG_FORK)
	{
		philo_memcpy(" took a fork\n", &msg_buffer[*msg_buf_i], 13);
		*msg_buf_i = *msg_buf_i + 13;
	}
	else if (msg_info->msg_type == MSG_EAT)
	{
		philo_memcpy(" is eating\n", &msg_buffer[*msg_buf_i], 11);
		*msg_buf_i = *msg_buf_i + 11;
	}
	else if (msg_info->msg_type == MSG_SLEEP)
	{
		philo_memcpy(" is sleeping\n", &msg_buffer[*msg_buf_i], 13);
		*msg_buf_i = *msg_buf_i + 13;
	}
	//printf("message buffer: %d\n", msg_buf_i);
	write(STDOUT_FILENO, &msg_buffer[0], *msg_buf_i);
	//printf("%s", msg_buffer);
	//printf("post msg: %lu\n", get_timestamp_in_ms(panopticon_data->start_timestamp));
	memset(msg_buffer, 0, *msg_buf_i + 1);
	*msg_buf_i = 0;
//	printf("*msg_buf: %d\n", *msg_buf_i);
	return (0);
}

//int	display_message(
//	t_panopticon_data *panopticon_data,
//	t_msg_info_local *msg_info
//)
//{
//	if (msg_info->msg_type == MSG_DEAD)
//	{
//		printf("%lu philosopher %d fucking died\n",
//				msg_info->timestamp, msg_info->philo_index + 1);
//		pthread_mutex_lock(panopticon_data->start->mutex);
//		panopticon_data->start->run_simulation = false;
//		pthread_mutex_unlock(panopticon_data->start->mutex);
//		return (1);
//	}	
//	else if (msg_info->msg_type == MSG_THINK)
//		printf("%lu philosopher %d is thinking\n",
//			msg_info->timestamp, msg_info->philo_index + 1);
//	else if (msg_info->msg_type == MSG_FORK)
//		printf("%lu philosopher %d took a fork\n",
//			msg_info->timestamp, msg_info->philo_index + 1);
//	else if (msg_info->msg_type == MSG_EAT)
//		printf("%lu philosopher %d is eating\n",
//			msg_info->timestamp, msg_info->philo_index + 1);
//	else if (msg_info->msg_type == MSG_SLEEP)
//		printf("%lu philosopher %d is sleeping\n",
//			msg_info->timestamp, msg_info->philo_index + 1);
//	printf("post msg: %lu\n", get_timestamp_in_ms(panopticon_data->start_timestamp));
//	return (0);
//}
//
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

	memset(msg_buffer, 0, MSG_BUF_MAX);
	printf("PANOPTICON.\n");
	pthread_mutex_lock(panopticon_data->start->mutex);
	if (panopticon_data->start->run_simulation == true)
		panopticon_data->start_timestamp = panopticon_data->start->timestamp;
	pthread_mutex_unlock(panopticon_data->start->mutex);
	i = 0;
	msg_buf_i = 0;
	unsigned long	loop_stamp;
	while (1)
	{
		// maybe replace with a looping usleep
		if (msg_buf_i == 0)
			loop_stamp = get_timestamp_in_ms(panopticon_data->start_timestamp);
		//usleep(2000);
		pthread_mutex_lock(&panopticon_data->msg_info->first_free_index_mutex);
		msg_info_local.first_free_index = panopticon_data->msg_info->first_free_index;
		pthread_mutex_unlock(&panopticon_data->msg_info->first_free_index_mutex);
		while (msg_info_local.first_free_index == goal)
		{
			usleep(2000);
			pthread_mutex_lock(&panopticon_data->msg_info->first_free_index_mutex);
			msg_info_local.first_free_index = panopticon_data->msg_info->first_free_index;
			pthread_mutex_unlock(&panopticon_data->msg_info->first_free_index_mutex);
		}
		// this probably goes in it;s own fucking thugn as ewkfbhdjsfsadjfs
		if (msg_info_local.first_free_index < i)
			goal = LOG_BUF_MAX;
		else
			goal = msg_info_local.first_free_index;
		printf("what is goal: %d\n", goal);
		while (i < goal)
		{
			assert(msg_buffer[msg_buf_i] == 0);
			get_log_values(panopticon_data, &msg_info_local, i);
			//if (display_message(panopticon_data, &msg_info_local) != 0)
			//	return (NULL);
			if (display_message(panopticon_data, &msg_info_local, msg_buffer, &msg_buf_i) != 0)
				return (NULL);
		//	else if (get_timestamp_in_ms(panopticon_data->start_timestamp) - loop_stamp > 20
		//		|| msg_buf_i > (MSG_BUF_MAX / 4) * 3)
		//	{
		//		//printf("message buffer: %d\n", msg_buf_i);
		//		write(STDOUT_FILENO, &msg_buffer[0], msg_buf_i + 1);
		//		//printf("%s", msg_buffer);
		//		//printf("post msg: %lu\n", get_timestamp_in_ms(panopticon_data->start_timestamp));
		//		memset(msg_buffer, 0, msg_buf_i + 1);
		//		msg_buf_i = 0;
		//	}
			//printf("i: %d, first free: %d\n", i, first_free_index);
			if (i == LOG_BUF_MAX - 1)
			{
				if (goal == LOG_BUF_MAX)
					goal = msg_info_local.first_free_index;
				i = 0;
			}
			else
				i++;
		}
	}
	return (NULL);
}
