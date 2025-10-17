/* ************************************************************************** */
/*                                                                            */
/*                                                            ::::::::        */
/*   message_buffer.c                                        :+:    :+:       */
/*                                                          +:+               */
/*   By: avaliull <avaliull@student.codam.nl>              +#+                */
/*                                                        +#+                 */
/*   Created: 2025/10/17 13:25:01 by avaliull            #+#    #+#           */
/*   Updated: 2025/10/17 13:31:17 by avaliull            ########   odam.nl   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	count_meals(
	t_panopticon_data *const panopticon_data,
	const int philo_i
)
{
	panopticon_data->meals_eaten[philo_i]++;
	if (panopticon_data->meals_eaten[philo_i] == panopticon_data->meal_count)
		panopticon_data->philos_sated++;
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
	{
		philo_memcpy(" died\n", &msg_buf->arr[msg_buf->i], 6);
		return (death);
	}
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
