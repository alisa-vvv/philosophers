// ************************************************************************** //
//                                                                            //
//                                                       ::::::::             //
//   panopticon.c                                      :+:    :+:             //
//                                                    +:+                     //
//   By: avaliull <avaliull@student.codam.nl>        +#+                      //
//                                                  +#+                       //
//   Created: 2025/09/19 16:26:03 by avaliull     #+#    #+#                  //
//   Updated: 2025/10/07 15:58:40 by avaliull     ########   odam.nl          //
//                                                                            //
// ************************************************************************** //

#include "philo.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void	log_and_write(
	t_panopticon_data *const panopticon_data,
	t_msg_buf *msg_buf
)
{
	int				i;
	int				loop_more;

	msg_buf->i = 0;
	i = 0;
	loop_more = 0;
	while (loop_more == 0)
	{
		memset(msg_buf, 0, MSG_BUF_MAX);
		usleep(MILLISECOND);
		loop_more = logger_loop(panopticon_data, msg_buf, &i);
		if (loop_more != 0)
		{
			//
			unsigned long stamp_pre = get_timestamp_in_ms(panopticon_data->start_timestamp);
			if (msg_buf->arr[0] != '\0')
				printf("%s", msg_buf->arr);
			unsigned long stamp_post = get_timestamp_in_ms(panopticon_data->start_timestamp);
			printf("death timing pre print: %lu, post print: %lu\n", stamp_pre, stamp_post);
			//
			pthread_mutex_lock(panopticon_data->start->mutex);
			panopticon_data->start->run_simulation = false;
			pthread_mutex_unlock(panopticon_data->start->mutex);
		}
	}
	return ;
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
	log_and_write(panopticon_data, &msg_buf);
	return (NULL);
}
