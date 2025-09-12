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
# include <pthread.h>

# define SECOND 1000000
# define MILLISECOND 1000

typedef	enum e_philo_errno
{
	success,
	wrong_argc,
	invalid_argument
}	t_philo_errno;

typedef struct s_philo_args
{
	unsigned long	philo_count;
	unsigned long	time_to_die;
	unsigned long	time_to_eat;
	unsigned long	time_to_sleep;
	int				meal_count;
}	t_philo_args;

typedef enum	e_fork
{
	USED = 1,
	UNUSED = 0,
	NEVER_USED = -1,
}	t_fork;

typedef enum	e_philo
{
	NONEXISTANCE = -1,
	THINKING = 0,
	EATING = 1,
	SLEEPING = 2,
}	t_philo;

typedef struct	s_forkex
{
	t_fork			fork;
	pthread_mutex_t	mutex;
}	t_forkex;

typedef struct	s_thread_data
{
	t_philo_args	philo_args;
	int 			philo_index;
	t_forkex		*left_forkex;
	t_forkex		*right_forkex;
	unsigned long	start_timestamp;
}	t_thread_data;


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

void	*philo_calloc(
	size_t nmemb,
	size_t size
);
/*		endof Utils		*/

/*		Test Functions (COMMENT THEM OUT)			*/
void			TEST_print_args(
	t_philo_args *args
);
/*		endof Test Functions						*/

#endif
