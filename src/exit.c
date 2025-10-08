/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   exit.c                                            :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/05/30 17:59:34 by avaliull     #+#    #+#                  */
/*   Updated: 2025/05/30 17:59:49 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo_errno	philo_exit(
	t_philo_errno error_code
)
{
	if (error_code == wrong_argc)
		philo_putstr_fd(STDERR_FILENO, "Error! Wrong argument count\n");
	else if (error_code == invalid_argument)
		philo_putstr_fd(STDERR_FILENO, "Error! Invalid argument(s)\n");
	else if (error_code == mutex_init_fail)
		philo_putstr_fd(STDERR_FILENO, "Error! Mutex init fail\n");
	else if (error_code == mutex_lock_fail)
		philo_putstr_fd(STDERR_FILENO, "Error! Mutex lock fail\n");
	else if (error_code == thread_create_fail)
		philo_putstr_fd(STDERR_FILENO, "Error! Thread create fail\n");
	else if (error_code == thread_join_fail)
		philo_putstr_fd(STDERR_FILENO, "Error! Thread create fail\n");
	return (error_code);
}
