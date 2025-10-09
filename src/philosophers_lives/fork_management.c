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
	t_thread_data *const episteme,
	t_forkex *const forkex,
	int *const forks_held
)
{
	unsigned long	timestamp;

	if (forkex->fork == UNUSED
		|| (forkex->fork == NEVER_USED && episteme->philo_i % 2 == 0))
	{
		forkex->fork = USED;
		(*forks_held)++;
		if (pthread_mutex_unlock(&forkex->mutex) != 0)
			return (mutex_unlock_fail);
		timestamp = get_timestamp(episteme->start_stamp);
		log_action(episteme, episteme->philo_i, MSG_FORK, timestamp); // NEED ERROR CHECK HERE
	}
	else
	{
		if (pthread_mutex_unlock(&forkex->mutex) != 0)
			return (mutex_unlock_fail);
	}
	return (0);
}

int	find_free_forks(
	t_thread_data *const episteme,
	int	*const forks_held
)
{
	int	err_check;

	if (pthread_mutex_lock(&episteme->left_forkex->mutex) != 0)
		return (mutex_lock_fail);
	err_check = take_a_fork(episteme, episteme->left_forkex, forks_held);
	if (err_check != 0)
		return (err_check);
	if (pthread_mutex_lock(&episteme->right_forkex->mutex) != 0)
		return (mutex_lock_fail);
	err_check = take_a_fork(episteme, episteme->right_forkex, forks_held);
	if (err_check != 0)
		return (err_check);
	return (success);
}
