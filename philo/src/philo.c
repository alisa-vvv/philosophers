/* ************************************************************************** */
/*                                                                            */
/*                                                            ::::::::        */
/*   philo.c                                                 :+:    :+:       */
/*                                                          +:+               */
/*   By: avaliull <avaliull@student.codam.nl>              +#+                */
/*                                                        +#+                 */
/*   Created: 2025/10/17 13:25:19 by avaliull            #+#    #+#           */
/*   Updated: 2025/10/17 13:49:24 by avaliull            ########   odam.nl   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <string.h>
#include <pthread.h> 

static void	msg_log_init(
	t_msg_log *msg_log
)
{
	int	i;

	i = -1;
	while (++i < LOG_BUF_MAX)
	{
		msg_log[i].msg_type = 0;
		msg_log[i].timestamp = 0;
		msg_log[i].philo_i = 0;
	}
}

static int	log_setup_sim_run(
	t_panopticon_data *const panopticon_data,
	t_thread_data *episteme,
	t_philo_args philo_args,
	t_start *start
)
{
	t_msg_log		msg_log[LOG_BUF_MAX];
	unsigned long	log_index;
	pthread_mutex_t	log_mutex;
	int				i;

	msg_log_init(msg_log);
	log_index = 0;
	if (pthread_mutex_init(&log_mutex, NULL) != 0)
		return (pthread_mutex_destroy(start->mutex), mutex_init_fail);
	panopticon_data->log_index = &log_index;
	panopticon_data->log_mutex = &log_mutex;
	panopticon_data->log = msg_log;
	i = -1;
	while (++i < philo_args.philo_count)
	{
		episteme[i].log_index = &log_index;
		episteme[i].log_mutex = &log_mutex;
		episteme[i].log = msg_log;
	}
	run_threads(episteme, panopticon_data, philo_args, start);
	pthread_mutex_destroy(start->mutex);
	pthread_mutex_destroy(&log_mutex);
	return (0);
}

/*	In science and philosophy, a paradigm is a distinct set of concepts or
 *	thought patterns, including theories, research methods, postulates, and
 *	standards for what constitute legitimate contributions to a field.
 *
 *	The philosophers are only able to work within what they know.
 *	Their paradigm consists entirely of forks, spaghetti, and the horrible
 *	knlowdge of what happens if they don't eat in time_to_die MS.
 *	This function constructs their paradigm for them. The philosophers will not
 *	live long enough to see it shift.
*/
static void	construct_paradigm(
	t_thread_data *episteme,
	t_philo_args philo_args,
	t_philo *philosophers,
	t_forkex *forkexes
)
{
	int	i;

	i = -1;
	while (++i < philo_args.philo_count)
	{
		episteme[i].philo = &philosophers[i];
		episteme[i].philo_count = philo_args.philo_count;
		episteme[i].time_to_die = philo_args.time_to_die;
		episteme[i].time_to_eat = philo_args.time_to_eat;
		episteme[i].time_to_sleep = philo_args.time_to_sleep;
		episteme[i].meal_count = philo_args.meal_count;
		episteme[i].left_forkex = &forkexes[i];
		if (i != philo_args.philo_count - 1)
			episteme[i].right_forkex = &forkexes[i + 1];
		else
			episteme[i].right_forkex = &forkexes[0];
		episteme[i].philo_i = i;
	}
}

static int	prepare_and_run_simulation(
	t_philo_args philo_args,
	t_philo *philosophers,
	t_forkex *forkexes
)
{
	t_start				start;
	pthread_mutex_t		start_mutex;
	t_thread_data		episteme[PHILO_BUF_MAX];
	t_panopticon_data	panopticon_data;
	int					i;

	if (pthread_mutex_init(&start_mutex, NULL) != 0)
		return (mutex_init_fail);
	start.timestamp = 0;
	start.run_simulation = false;
	start.timestamp = 0;
	start.mutex = &start_mutex;
	panopticon_data.start = &start;
	i = -1;
	while (++i < philo_args.philo_count)
		episteme[i].start = &start;
	memset(panopticon_data.meals_eaten, 0, PHILO_BUF_MAX);
	panopticon_data.philo_count = philo_args.philo_count;
	panopticon_data.meal_count = philo_args.meal_count;
	panopticon_data.philos_sated = 0;
	panopticon_data.start = &start;
	construct_paradigm(episteme, philo_args, philosophers, forkexes);
	return (log_setup_sim_run(&panopticon_data, episteme, philo_args, &start));
}

int	main(
	int argc,
	char **argv
)
{
	t_philo_errno	err_check;
	t_philo_args	philo_args;
	t_philo			philosophers[PHILO_BUF_MAX];
	t_forkex		forkexes[PHILO_BUF_MAX];
	int				i;

	if (argc != 5 && argc != 6)
		return (philo_exit(wrong_argc));
	err_check = set_philo_args(&philo_args, argv);
	if (err_check != success)
		return (philo_exit(err_check));
	err_check = init_mutexes_philos(philo_args, philosophers, forkexes);
	if (err_check != 0)
		philo_exit(err_check);
	err_check = prepare_and_run_simulation(philo_args, philosophers, forkexes);
	if (err_check != 0)
		philo_exit(err_check);
	i = -1;
	while (++i < philo_args.philo_count)
		pthread_mutex_destroy(&forkexes[i].mutex);
	philo_exit(success);
}
