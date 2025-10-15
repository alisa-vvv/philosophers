// ************************************************************************** //
//                                                                            //
//                                                       ::::::::             //
//   message_buffer.c                                  :+:    :+:             //
//                                                    +:+                     //
//   By: avaliull <avaliull@student.codam.nl>        +#+                      //
//                                                  +#+                       //
//   Created: 2025/10/01 14:55:03 by avaliull     #+#    #+#                  //
//   Updated: 2025/10/07 16:09:30 by avaliull     ########   odam.nl          //
//                                                                            //
// ************************************************************************** //

#include <stdio.h>
#include <string.h>

#include "philo.h"

static int	handle_death(
	t_panopticon_data *const panopticon_data,
	t_msg_buf *msg_buf
)
{
	printf("%s\n", msg_buf->arr);
	philo_memcpy(" died\n", &msg_buf->arr[msg_buf->i], 6);
	//write(STDOUT_FILENO, msg_buf->arr, msg_buf->i + 7);
	pthread_mutex_lock(panopticon_data->start->mutex);
	panopticon_data->start->run_simulation = false;
	pthread_mutex_unlock(panopticon_data->start->mutex);
	return (1);
}

static void	count_meals(
	t_panopticon_data *const panopticon_data,
	const int	philo_i
)
{
//	printf("philo: %d\n", philo_i);
//	printf("meals eaten before: %d\n", panopticon_data->meals_eaten[philo_i]);
	panopticon_data->meals_eaten[philo_i]++;
//	printf("meals eaten after: %d\n", panopticon_data->meals_eaten[philo_i]);
	if (panopticon_data->meals_eaten[philo_i] == panopticon_data->meal_count)
	{
//		printf("panopticon_data->philos_sated before: %d\n", panopticon_data->philos_sated);
		panopticon_data->philos_sated++;
//		printf("panopticon_data->philos_sated after: %d\n", panopticon_data->philos_sated);
	}
	if (panopticon_data->philos_sated == panopticon_data->philo_count)
	{
		pthread_mutex_lock(panopticon_data->start->mutex);
		panopticon_data->start->run_simulation = false;
		pthread_mutex_unlock(panopticon_data->start->mutex);
	}
}

int	log_to_str(
	t_panopticon_data *const panopticon_data,
	t_msg_info *msg_info,
	t_msg_buf *msg_buf
)
{
	msg_buf->i += philo_ultoa(msg_info->timestamp, &msg_buf->arr[msg_buf->i]);
	philo_memcpy(" philosopher ", &msg_buf->arr[msg_buf->i], 13);
	msg_buf->i += 13;
	msg_buf->i += philo_ultoa(msg_info->philo_i + 1, &msg_buf->arr[msg_buf->i]);
	if (msg_info->msg_type == MSG_DEAD)
		return (handle_death(panopticon_data, msg_buf));
	else if (msg_info->msg_type == MSG_THINK)
		philo_memcpy(" is thinking\n", &msg_buf->arr[msg_buf->i], 13);
	else if (msg_info->msg_type == MSG_FORK)
		philo_memcpy(" took a fork\n", &msg_buf->arr[msg_buf->i], 13);
	else if (msg_info->msg_type == MSG_EAT)
	{
		philo_memcpy(" is eating\n", &msg_buf->arr[msg_buf->i], 11);
		count_meals(panopticon_data, msg_info->philo_i);
	}
	else if (msg_info->msg_type == MSG_SLEEP)
		philo_memcpy(" is sleeping\n", &msg_buf->arr[msg_buf->i], 13);
	if (msg_info->msg_type == MSG_EAT)
		msg_buf->i += 11;
	else
		msg_buf->i += 13;
	return (0);
}
