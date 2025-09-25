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
	pthread_mutex_lock(panopticon_data->log_mutex);
	local_info->msg_type = panopticon_data->log_arr[i];
	panopticon_data->log_arr[i] = 0;
	local_info->timestamp = panopticon_data->log_arr[i + 1];
	panopticon_data->log_arr[i + 1] = 0;
	local_info->philo_index = panopticon_data->log_arr[i + 2];
	panopticon_data->log_arr[i + 2] = 0;
	pthread_mutex_unlock(panopticon_data->log_mutex);
}

int	philo_ultoa(
	unsigned long	var,
	char *str
)
{
	unsigned long	temp_var;
	int				var_index;
	int				number_len;

	if (var == 0)
	{
		*str = '0';
		return (1);
	}
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
	int	*i
)
{
	unsigned long TEST_post_stamp = 0;
	unsigned long TEST_pre_stamp = 0;

	*i += philo_ultoa(msg_info->timestamp, &msg_buffer[*i]);
	philo_memcpy(" philosopher ", &msg_buffer[*i], 13);
	*i = *i + 13;
	*i = *i + philo_ultoa(msg_info->philo_index + 1, &msg_buffer[*i]);
	if (msg_info->msg_type == MSG_DEAD)
	{
		philo_memcpy(" died\n", &msg_buffer[*i], 6);
		//*i = *i + 6;
		TEST_pre_stamp = get_timestamp_in_ms(panopticon_data->start_timestamp);
		write(STDOUT_FILENO, msg_buffer, *i + 7);
	//	printf("%s", msg_buffer);
		//fflush(stdout);
		//printf("%s", msg_buffer);
		TEST_post_stamp = get_timestamp_in_ms(panopticon_data->start_timestamp);
		
		write(STDOUT_FILENO, "\npost msg: ", 11);
		char	post_msg[24];
		memset(post_msg, 0, 24);
		philo_ultoa(TEST_pre_stamp, post_msg);
		write(STDOUT_FILENO, post_msg, 5);
		memset(post_msg, 0, 24);
		write(STDOUT_FILENO, "\npost msg: ", 11);
		philo_ultoa(TEST_post_stamp, post_msg);
		write(STDOUT_FILENO, post_msg, 5);

		pthread_mutex_lock(panopticon_data->start->mutex);
		panopticon_data->start->run_simulation = false;
		pthread_mutex_unlock(panopticon_data->start->mutex);
		return (1);
	}	
	else if (msg_info->msg_type == MSG_THINK)
	{
		philo_memcpy(" is thinking\n", &msg_buffer[*i], 13);
		*i = *i + 13;
	}
	else if (msg_info->msg_type == MSG_FORK)
	{
		philo_memcpy(" took a fork\n", &msg_buffer[*i], 13);
		*i = *i + 13;
	}
	else if (msg_info->msg_type == MSG_EAT)
	{
		philo_memcpy(" is eating\n", &msg_buffer[*i], 11);
		*i = *i + 11;
		(panopticon_data->meals_eaten[msg_info->philo_index])++;
		//printf("philo num: %d, meals eaean: %d\n", msg_info->philo_index, panopticon_data->meals_eaten[msg_info->philo_index]);
		if (panopticon_data->meals_eaten[msg_info->philo_index] == panopticon_data->meal_count)
			panopticon_data->philos_sated = panopticon_data->philos_sated + 1;
	}
	else if (msg_info->msg_type == MSG_SLEEP)
	{
		philo_memcpy(" is sleeping\n", &msg_buffer[*i], 13);
		*i = *i + 13;
	}
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
	printf("PANOPTICON.\n");
	pthread_mutex_lock(panopticon_data->start->mutex);
	if (panopticon_data->start->run_simulation == true)
		panopticon_data->start_timestamp = panopticon_data->start->timestamp;
	pthread_mutex_unlock(panopticon_data->start->mutex);
	i = 0;
	msg_buf_i = 0;
	// MAKE SURE TO MAKE ITERATIONS BE += 3
	while (1)
	{
		//write(STDOUT_FILENO, "here?\n", 6);
		// maybe replace with a looping usleep
		if (msg_buf_i == 0)
			loop_stamp = get_timestamp_in_ms(panopticon_data->start_timestamp);
		//while (get_timestamp_in_ms(panopticon_data->start_timestamp) - loop_stamp < 7)
		usleep(1000);
		pthread_mutex_lock(panopticon_data->log_mutex);
		msg_info_local.log_index = *panopticon_data->log_index;
		pthread_mutex_unlock(panopticon_data->log_mutex);
		if (msg_info_local.log_index < i)
			goal = LOG_BUF_MAX;
		else
			goal = msg_info_local.log_index;
		//printf("what is goal: %d\n", goal);
		while (i < goal)
		{
			//printf("goal: %d\n", goal);
			assert(msg_buffer[msg_buf_i] == 0);
			get_log_values(panopticon_data, &msg_info_local, i);
			if (display_message(panopticon_data, &msg_info_local, msg_buffer, &msg_buf_i) != 0)
				return (NULL);
			else if (get_timestamp_in_ms(panopticon_data->start_timestamp) - loop_stamp > 25
					|| msg_buf_i > (MSG_BUF_MAX / 4) * 3)
				write_and_clear_msg_buf(msg_buffer, &msg_buf_i);
			if (panopticon_data->philos_sated == panopticon_data->philo_count)
				return (write_and_clear_msg_buf(msg_buffer, &msg_buf_i));
		//	pthread_mutex_lock(panopticon_data->start->mutex);
		//	pthread_mutex_unlock(panopticon_data->start->mutex); // why is this here?
			if (i == LOG_BUF_MAX - 3)
			{
				if (goal == LOG_BUF_MAX)
					goal = msg_info_local.log_index;
				i = 0;
			}
			else
				i += 3;
		}
		pthread_mutex_lock(panopticon_data->start->mutex);
		if (panopticon_data->start->run_simulation == false)
		{
			pthread_mutex_unlock(panopticon_data->start->mutex);
			return (write_and_clear_msg_buf(msg_buffer, &msg_buf_i));
		}
		pthread_mutex_unlock(panopticon_data->start->mutex);
	}
	return (NULL);
}
