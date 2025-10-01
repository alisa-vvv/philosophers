// ************************************************************************** //
//                                                                            //
//                                                       ::::::::             //
//   message_buffer.c                                  :+:    :+:             //
//                                                    +:+                     //
//   By: avaliull <avaliull@student.codam.nl>        +#+                      //
//                                                  +#+                       //
//   Created: 2025/10/01 14:55:03 by avaliull     #+#    #+#                  //
//   Updated: 2025/10/01 15:41:18 by avaliull     ########   odam.nl          //
//                                                                            //
// ************************************************************************** //

#include <stdio.h>

#include <string.h>
#include "philo.h"

int	log_to_str(
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
	*i = *i + philo_ultoa(msg_info->philo_i + 1, &msg_buffer[*i]);
	if (msg_info->msg_type == MSG_DEAD)
	{
		pthread_mutex_lock(panopticon_data->start->mutex);
		panopticon_data->start->run_simulation = false;
		pthread_mutex_unlock(panopticon_data->start->mutex);
		//usleep(100);
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

		return (1);
	}	
	else if (msg_info->msg_type == MSG_THINK)
		philo_memcpy(" is thinking\n", &msg_buffer[*i], 13);
	else if (msg_info->msg_type == MSG_FORK)
		philo_memcpy(" took a fork\n", &msg_buffer[*i], 13);
	else if (msg_info->msg_type == MSG_EAT)
	{
		philo_memcpy(" is eating\n", &msg_buffer[*i], 11);
		(panopticon_data->meals_eaten[msg_info->philo_i])++;
		if (panopticon_data->meals_eaten[msg_info->philo_i] == panopticon_data->meal_count)
		{
			printf("philo %d ate %d meals\n", msg_info->philo_i, panopticon_data->meals_eaten[msg_info->philo_i]);
			panopticon_data->philos_sated = panopticon_data->philos_sated + 1;
		}
	}
	else if (msg_info->msg_type == MSG_SLEEP)
		philo_memcpy(" is sleeping\n", &msg_buffer[*i], 13);
	if (msg_info->msg_type == MSG_EAT)
		*i = *i + 11;
	else
		*i = *i + 13;
	return (0);
}
