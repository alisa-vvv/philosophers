/* ************************************************************************** */
/*                                                                            */
/*                                                            ::::::::        */
/*   panopticon.c                                            :+:    :+:       */
/*                                                          +:+               */
/*   By: avaliull <avaliull@student.codam.nl>              +#+                */
/*                                                        +#+                 */
/*   Created: 2025/10/17 13:25:11 by avaliull            #+#    #+#           */
/*   Updated: 2025/10/17 13:42:01 by avaliull            ########   odam.nl   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

/* The panopticon is a design of institutional building with an inbuilt system
 * of control, originated by the English philosopher and social theorist Jeremy
 * Bentham in the 18th century. The concept is to allow all prisoners of an
 * institution to be observed by a single prison officer, without the inmates
 * knowing whether or not they are being watched. 
 *
 * This function transforms the log data into a string, and prints it every 1 ms
*/
void	panopticon(
	t_panopticon_data *const panopticon_data
)
{
	t_msg_buf		msg_buf;
	int				i;
	int				loop_more;

	msg_buf.i = 0;
	i = 0;
	loop_more = 0;
	while (loop_more == 0)
	{
		memset(msg_buf.arr, 0, MSG_BUF_MAX);
		loop_more = logger_loop(panopticon_data, &msg_buf, &i);
		if (loop_more != 0)
		{
			if (msg_buf.arr[0] != '\0')
				printf("%s", msg_buf.arr);
			pthread_mutex_lock(panopticon_data->start->mutex);
			panopticon_data->start->run_simulation = false;
			pthread_mutex_unlock(panopticon_data->start->mutex);
		}
	}
	msg_buf.i = 0;
}
