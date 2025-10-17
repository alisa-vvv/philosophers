/* ************************************************************************** */
/*                                                                            */
/*                                                            ::::::::        */
/*   fork_management.c                                       :+:    :+:       */
/*                                                          +:+               */
/*   By: avaliull <avaliull@student.codam.nl>              +#+                */
/*                                                        +#+                 */
/*   Created: 2025/10/17 13:24:22 by avaliull            #+#    #+#           */
/*   Updated: 2025/10/17 13:28:19 by avaliull            ########   odam.nl   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// add a "first take function?
//
int	take_a_fork(
	t_thread_data *episteme,
	t_forkex *forkex,
	int *forks_held
)
{
	// removed unused markers?
	unsigned long	timestamp;

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
	int	*forks_held,
	unsigned long *last_eaten
)
{
	while (*forks_held != 2)
	{
		usleep(1000);
		if (check_if_dead(episteme, last_eaten) == 1)
			return (1);
		pthread_mutex_lock(&episteme->left_forkex->mutex);
		take_a_fork(episteme, episteme->left_forkex, forks_held);
		pthread_mutex_lock(&episteme->right_forkex->mutex);
		take_a_fork(episteme, episteme->right_forkex, forks_held);
	}
	return (0);
}
