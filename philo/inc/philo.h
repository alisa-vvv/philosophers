/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avaliull <avaliull@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 15:41:30 by avaliull          #+#    #+#             */
/*   Updated: 2025/10/17 14:18:14 by avaliull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <pthread.h>
# include <stdbool.h>

# define NO_LIMIT -1
# define MAX_MSG_LEN 40 /*
	this value is 39 because the largest possible message is:
	4294967295 philosopher 200 is thinking\n
	which is 39 characters. the value is bigger cause it's realted to an index.
*/
# define PHILO_BUF_MAX 200
# define MSG_BUF_MAX 16384
# define LOG_BUF_MAX 8192

typedef enum e_philo_errno
{
	success = 0,
	death = 1,
	buf_overflow = -1,
	wrong_argc = 2,
	invalid_argument = 3,
	too_many_philos = 4,
}	t_philo_errno;

typedef struct s_philo_args
{
	unsigned long	philo_count;
	unsigned long	time_to_die;
	unsigned long	time_to_eat;
	unsigned long	time_to_sleep;
	int				meal_count;
}	t_philo_args;

typedef enum e_fork
{
	USED = 1,
	UNUSED = 0,
	NEVER_USED = -1,
}	t_fork;
typedef struct s_forkex
{
	t_fork			fork;
	pthread_mutex_t	mutex;
}	t_forkex;

typedef enum e_philo
{
	NONEXISTANCE = -1,
	THINKING = 0,
	EATING = 1,
	SLEEPING = 2,
}	t_philo;

/*		Message Buffer		*/
typedef enum e_msg_type
{
	MSG_NULL = 0,
	MSG_THINK = 1,
	MSG_FORK = 2,
	MSG_EAT = 3,
	MSG_SLEEP = 4,
	MSG_DEAD = 5,
	DONE_EATING = 6,
}	t_msg_type;
typedef struct s_msg_info
{
	t_msg_type		msg_type;
	unsigned long	timestamp;
	int				philo_i;
	int				log_index;
}	t_msg_info;
typedef struct s_msg_log
{
	t_msg_type		msg_type;
	unsigned long	timestamp;
	int				philo_i;
}	t_msg_log;
typedef struct s_msg_buf
{
	int		i;
	char	arr[MSG_BUF_MAX];
}	t_msg_buf;
/*	endof Message Buffer	*/

/*	Simulation start struct */
typedef struct s_start
{
	unsigned long	timestamp;
	bool			run_simulation;
	pthread_mutex_t	*mutex;
}	t_start;

/*	Logger struct */
typedef struct s_panopticon_data
{
	unsigned long	*log_index;
	t_msg_log		*log;
	pthread_mutex_t	*log_mutex;
	int				meals_eaten[PHILO_BUF_MAX];
	int				philos_sated;
	int				philo_count;
	int				meal_count;
	t_start			*start;
	unsigned long	start_timestamp;
}	t_panopticon_data;

/*	Thread data struct */
typedef struct s_thread_data
{
	unsigned long	philo_count;
	unsigned long	time_to_die;
	unsigned long	time_to_eat;
	unsigned long	time_to_sleep;
	int				meal_count;
	t_start			*start;
	unsigned long	start_timestamp;
	int				philo_i;
	t_philo			*philo;
	t_forkex		*left_forkex;
	t_forkex		*right_forkex;
	unsigned long	*log_index;
	t_msg_log		*log;
	pthread_mutex_t	*log_mutex;
}	t_thread_data;

/*		Philo threads	*/
int			
	run_threads(
		t_thread_data *episteme,
		t_panopticon_data *panopticon_data,
		t_philo_args philo_args,
		t_start *start
		);
/*	endof Philo threads	*/

/*		States		*/
int			
	check_simulation_end(
		t_thread_data	*episteme
		);
int			
	check_if_dead(
		t_thread_data *episteme,
		unsigned long *last_eaten
		);
int			
	philo_think(
		t_thread_data *episteme
		);
int			
	philo_sleep(
		t_thread_data *episteme,
		unsigned long *last_eaten,
		int philo_i
		);
int			
	philo_eat(
		t_thread_data *episteme,
		unsigned long *last_eaten,
		int philo_i,
		int *forks_held
		);
/*	endof States	*/

/*		Fork management		*/
int			
	take_a_fork(
		t_thread_data *episteme,
		t_forkex *forkex,
		int *forks_held
		);
int			
	find_free_forks(
		t_thread_data *episteme,
		int	*forks_held,
		unsigned long *last_eaten
		);
/*	endof Fork management	*/

/*		Panopticon		*/
void		
	panopticon(
		t_panopticon_data *const panopticon_data
		);
int			
	logger_loop(
		t_panopticon_data *const panopticon_data,
		t_msg_buf *msg_buf,
		int	*i
		);
/*	endof Panopticon	*/

/*		Timestamping		*/
unsigned long	get_start_timestamp(void);
unsigned long
	get_timestamp_in_ms(
		unsigned long start_timestamp
		);
int			
	log_to_str(
		t_panopticon_data *panopticon_data,
		t_msg_info *msg_info,
		t_msg_buf *msg_buf
		);
void		
	log_action(
		t_thread_data *episteme,
		int philo_i,
		t_msg_type msg_type,
		unsigned long timestamp
		);
/*	endof Timestamping		*/

/*		Variable setup		*/
int			
	instantiate_subjects_and_objects(
		t_philo_args philo_args,
		t_philo *philosophers,
		t_forkex *forkexes
		);
t_philo_errno
	set_philo_args(
		t_philo_args *args,
		char **argv
		);
/*	endof Variable setup	*/

/*		Exit			*/
t_philo_errno
	philo_exit(
		t_philo_errno error_code
		);
/*	endof Exit			*/

/*		Utils			*/
int			
	philo_putstr_fd(
		int fd,
		char *str
		);
void		
	philo_memcpy(
		char *src,
		char *dest,
		int count
		);
int			
	philo_ultoa(
		unsigned long var,
		char *str
		);
int			
	philo_putstr_fd(
		int fd,
		char *str
		);
int			
	philo_atoi(
		unsigned char *str
		);
/*	endof Utils			*/

#endif
