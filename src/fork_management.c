// ************************************************************************** //
//                                                                            //
//                                                       ::::::::             //
//   fork_management.c                                 :+:    :+:             //
//                                                    +:+                     //
//   By: avaliull <avaliull@student.codam.nl>        +#+                      //
//                                                  +#+                       //
//   Created: 2025/09/15 19:46:02 by avaliull     #+#    #+#                  //
//   Updated: 2025/10/07 16:24:02 by avaliull     ########   odam.nl          //
//                                                                            //
// ************************************************************************** //

#include "philo.h"

int	take_a_fork(
	t_thread_data *episteme,
	t_forkex *forkex,
	int *forks_held
)
{
	unsigned long	timestamp;

	if (check_simulation_end(episteme) == 1)
		return (1);
	if (forkex->fork == UNUSED
			|| (forkex->fork == NEVER_USED && episteme->philo_i % 2 == 0))
	{
		forkex->fork = USED;
		(*forks_held)++;
		pthread_mutex_unlock(&forkex->mutex);
		timestamp = get_timestamp_in_ms(episteme->start_timestamp);
		log_action(episteme, episteme->philo_i, MSG_FORK, timestamp);
	}
	else
		pthread_mutex_unlock(&forkex->mutex);
	return (0);
}

int	find_free_forks(
	t_thread_data *episteme,
	int	*forks_held
)
{
	if (check_simulation_end(episteme) == 1)
		return (1);
	pthread_mutex_lock(&episteme->left_forkex->mutex);
	take_a_fork(episteme, episteme->left_forkex, forks_held);
	pthread_mutex_lock(&episteme->right_forkex->mutex);
	take_a_fork(episteme, episteme->right_forkex, forks_held);
	return (0);
}
