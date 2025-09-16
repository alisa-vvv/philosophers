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
# define NO_LIMIT -1

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

// for future message buffer integration
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

typedef struct	s_panopticon_data
{
	int	philo_count;
	int	meal_count;
}	t_panopticon_data;

typedef struct	s_msg_buf
{
	// when a philo's state changes, it should mutex lock this
	// then update last_free_index
	// last free index should loop around to 0 if max is reached
	// then mutex unlock the variable
	// then mutex lock the section it's currenty writing to
	// then write its changed values to previous last_free_index
	// then mutex unlock it
	// 
	// panopticon thread will iteratively check for new variables
	// probably with some usleeping in between
	// transform them into an actual message on the message array
	// then check if (some arbitrary small timeframe) has passed
	// then write the whole array
	// then memset it to 0
	// them mutex lock all that is before last_free_index
	// then memset all arrays to 0 up until current last_free_index
	// unlock
	// keep going forever
	int				philo_index[1024];
	int				last_free_index;
	unsigned long	timestamp[1024];
	t_msg_type		msg_type[1024];
}	t_msg_buf;

//char	messsage_arr[4096];
//

typedef struct	s_thread_data
{
	t_philo_args	philo_args;
	t_philo			*philo;
	int 			philo_index;
	t_forkex		*left_forkex;
	t_forkex		*right_forkex;
	unsigned long	start_timestamp;
}	t_thread_data;


/*		States		*/
void	philo_think(
	t_thread_data *episteme
);

void	philo_sleep(
	int	philo_index,
	t_philo	*philo,
	unsigned long start_timestamp,
	unsigned long time_to_sleep
);

void	philo_eat(
	t_thread_data *episteme,
	unsigned long *last_eaten,
	int	philo_index,
	int *forks_held
);
/*	endof States	*/

/*		Fork management		*/
void	take_a_fork(
	t_thread_data *episteme,
	t_forkex *forkex,
	int *forks_held
);

void	find_free_forks(
	t_thread_data *episteme,
	int	*forks_held
);
/*	endof Fork management	*/

/*		Timestamping		*/
unsigned long	get_start_timestamp(
	void
);

unsigned long	get_timestamp_in_ms(
	unsigned long	start_timestamp
);
/*	endof Timestamping		*/

/*		Variable setup		*/
int	construct_paradigm(
	t_thread_data *episteme,
	t_philo *philosophers,
	t_philo_args philo_args,
	t_forkex *forkexes
);

int	instantiate_subjects_and_objects(
	t_philo_args philo_args,
	t_philo **philosophers,
	t_forkex **forkexes
);

t_philo_errno	set_philo_args(
	t_philo_args *args,
	char **argv
);
/*	endof Variable setup	*/

/*		Exit			*/
t_philo_errno	philo_exit(
	t_philo_errno	error_code
);
/*	endof Exit			*/

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
