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

#include <assert.h>

#include "philo.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

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
	loop_stamp = get_timestamp(panopticon_data->start_stamp);
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
	t_panopticon_data *const	panopticon_data = (t_panopticon_data *) data;
	t_msg_buf					msg_buf;
	int							loop_check;

	memset(msg_buf.arr, 0, MSG_BUF_MAX);
	msg_buf.i = 0;
	pthread_mutex_lock(panopticon_data->start->mutex);
	if (panopticon_data->start->run_simulation == true)
		panopticon_data->start_stamp = panopticon_data->start->timestamp;
	pthread_mutex_unlock(panopticon_data->start->mutex);
	log_and_write(panopticon_data, &msg_buf);
	return (NULL);
}
