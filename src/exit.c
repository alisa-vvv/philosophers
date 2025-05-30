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
	t_philo_errno	error_code
)
{
	if (error_code == success)
		philo_putstr_fd(STDERR_FILENO, "Successful Exit\n");
	else if (error_code == wrong_argc)
		philo_putstr_fd(STDERR_FILENO, "Error! Wrong argument count\n");
	else if (error_code == invalid_argument)
		philo_putstr_fd(STDERR_FILENO, "Error! Invalid argument(s)\n");
	return (error_code);
}
