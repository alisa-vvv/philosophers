/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   philo.h                                           :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/05/30 15:41:30 by avaliull     #+#    #+#                  */
/*   Updated: 2025/05/30 18:00:44 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>

# define SECOND 1000000

typedef struct s_philo_args
{
	int	philo_count;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	meal_count;
}	t_philo_args;

typedef	enum e_philo_errno
{
	success,
	wrong_argc,
	invalid_argument
}	t_philo_errno;

/*		Exit			*/
t_philo_errno	philo_exit(
	t_philo_errno	error_code
);
/*		endof Exit		*/

/*		Utils			*/
int	philo_putstr_fd(
	int		fd,
	char	*str
);

int		philo_atoi(
	unsigned char	*str
);
/*		endof Utils		*/

/*		Test Functions (COMMENT THEM OUT)			*/
void			test_print_args(
	t_philo_args *args
);
/*		endof Test Functions						*/

#endif
