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
# include <stdbool.h>

# define SECOND 1000000
# define MILLISECOND 1000
# define NO_LIMIT -1
# define PHILO_BUF_MAX 256
# define MSG_BUF_MAX 8192
# define LOG_BUF_MAX 12288

typedef	enum e_philo_errno
{
	success,
	death,
	wrong_argc,
	invalid_argument,
	mutex_init_fail,
	mutex_lock_fail,
	mutex_unlock_fail,
	thread_create_fail,
	thread_join_fail,
	log_buf_overflow,
	msg_buf_overflow,
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

/*		Message Buffer		*/
typedef enum	e_msg_type
{
	MSG_NULL = 0,
	MSG_THINK = 1,
	MSG_FORK = 2,
	MSG_EAT = 3,
	MSG_SLEEP = 4,
	MSG_DEAD = 5,
	DONE_EATING = 6,
}	t_msg_type;
typedef struct	s_msg_info
{
	t_msg_type		msg_type;
	unsigned long	timestamp;
	int				philo_i;
	int				log_index;
}	t_msg_info;
typedef struct	s_start
{
	unsigned long	timestamp;
	bool			run_simulation;
	pthread_mutex_t	*mutex;
}	t_start;
typedef struct	s_msg_buf
{
	int		i;
	char	arr[MSG_BUF_MAX];
}	t_msg_buf;
/*	endof Message Buffer	*/

typedef struct	s_panopticon_data
{
	unsigned long	*log_arr;
	unsigned long	*log_index;
	pthread_mutex_t	*log_mutex;

	int				meals_eaten[PHILO_BUF_MAX];
	int				philos_sated;
	int				philo_count;
	int				meal_count;

	t_start			*start;

	unsigned long	start_stamp;
}	t_panopticon_data;


typedef struct	s_thread_data
{
	unsigned long	philo_count;
	unsigned long	time_to_die;
	unsigned long	time_to_eat;
	unsigned long	time_to_sleep;

	int				meal_count;

	t_start			*start;
	unsigned long	start_stamp;

	int 			philo_i;
	t_philo			*philo;
	t_forkex		*left_forkex;
	t_forkex		*right_forkex;

	unsigned long	*log_index;
	unsigned long	*log_arr;
	pthread_mutex_t	*log_mutex;

}	t_thread_data;


/*		Philo threads	*/
int	run_threads(
	t_thread_data *const episteme,
	t_panopticon_data *const panopticon_data,
	const t_philo_args philo_args,
	t_start *const start
);
void	*praxis(
	void *data
);
/*	endof Philo threads	*/

/*		States		*/
int	check_if_dead(
	t_thread_data *const episteme,
	unsigned long *const last_eaten
);
void	philo_think(
	t_thread_data *const episteme
);
int	philo_sleep(
	t_thread_data *const episteme,
	unsigned long *const last_eaten,
	int philo_i
);
int	philo_eat(
	t_thread_data *const episteme,
	unsigned long *const last_eaten,
	int	philo_i,
	int *const forks_held
);
/*	endof States	*/

/*		Fork management		*/
int	take_a_fork(
	t_thread_data *const episteme,
	t_forkex *const forkex,
	int *const forks_held
);
int	find_free_forks(
	t_thread_data *const episteme,
	int	*const forks_held
);
/*	endof Fork management	*/

/*		Panopticon		*/
void	*panopticon(
	void *data
);
int	logger_loop(
	t_panopticon_data *const panopticon_data,
	t_msg_buf *const msg_buf,
	int	*const i,
	unsigned long *const loop_stamp
);
/*	endof Panopticon	*/

/*		Timestamping		*/
unsigned long	get_start_stamp(
	void
);
unsigned long	get_timestamp(
	unsigned long	start_stamp
);
int	log_to_str(
	t_panopticon_data *const panopticon_data,
	t_msg_info *const msg_info,
	t_msg_buf *const msg_buf
);
int	log_action(
	t_thread_data *const episteme,
	int philo_i,
	t_msg_type msg_type,
	unsigned long timestamp
);
/*	endof Timestamping		*/

/*		Variable setup		*/
int	instantiate_subjects_and_objects(
	t_philo_args philo_args,
	t_philo *const philosophers,
	t_forkex *const forkexes
);
t_philo_errno	set_philo_args(
	t_philo_args *const args,
	const char *const *const argv
);
/*	endof Variable setup	*/

/*		Exit			*/
t_philo_errno	philo_exit(
	t_philo_errno	error_code
);
/*	endof Exit			*/

/*		Utils			*/
int	philo_putstr_fd(
	int fd,
	char *str
);
void	philo_memcpy(
	char *src,
	char *dest,
	int count
);
int	philo_ultoa(
	unsigned long var,
	char *const str
);
int	philo_putstr_fd(
	int fd,
	char *const str
);
int		philo_atoi(
	unsigned char *const str
);
/*	endof Utils			*/

/*		Test Functions (COMMENT THEM OUT)			*/
void			TEST_print_args(
	t_philo_args *args
);
/*		endof Test Functions						*/

#endif
