// ************************************************************************** //
//                                                                            //
//                                                       ::::::::             //
//   time_tracking.c                                   :+:    :+:             //
//                                                    +:+                     //
//   By: avaliull <avaliull@student.codam.nl>        +#+                      //
//                                                  +#+                       //
//   Created: 2025/09/15 19:39:18 by avaliull     #+#    #+#                  //
//   Updated: 2025/09/15 19:40:58 by avaliull     ########   odam.nl          //
//                                                                            //
// ************************************************************************** //

#include <sys/time.h>
#include <stddef.h>

unsigned long	get_timestamp_in_ms(
	unsigned long	start_timestamp
)
{
	struct timeval	current_time;
	unsigned long	new_timestamp;

	gettimeofday(&current_time, NULL);
	new_timestamp = current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
	new_timestamp -= start_timestamp;
	return (new_timestamp);
}

unsigned long	get_start_timestamp(
	void
)
{
	struct timeval	timestamp;
	unsigned long	start_timestamp;

	gettimeofday(&timestamp, NULL);
	start_timestamp = timestamp.tv_sec * 1000 + timestamp.tv_usec / 1000;
	return (start_timestamp);
}

