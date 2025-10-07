#include "philo.h"
#include <string.h>
#include <pthread.h> 

void	prepare_surveillance_data(
	t_panopticon_data *panopticon_data,
	unsigned long *log_arr,
	t_philo_args philo_args,
	t_start *start
)
{
	memset(log_arr, 0, LOG_BUF_MAX);
	memset(panopticon_data->meals_eaten, 0, PHILO_BUF_MAX);
	panopticon_data->log_arr = log_arr;
	panopticon_data->philo_count = philo_args.philo_count;
	panopticon_data->meal_count = philo_args.meal_count;
	panopticon_data->philos_sated = 0;
	panopticon_data->start = start;
}

// 	1. mutex init, add to panopticon and episteme data, calls:
//		2. setup log stuff, add to panopticon and episteme
//			3. fill values?
//				4. call start
int	prepare_simulation(
	t_philo_args philo_args,
	t_philo *philosophers,
	t_forkex *forkexes
)
{
	t_start				start;
	pthread_mutex_t		start_mutex;
	t_thread_data		episteme[PHILO_BUF_MAX];
	t_panopticon_data	panopticon_data;

	unsigned long		log_arr[LOG_BUF_MAX];
	unsigned long		log_index;
	pthread_mutex_t		log_mutex;

	pthread_mutex_init(&start_mutex, NULL);
	pthread_mutex_init(&log_mutex, NULL);
	start.run_simulation = false;
	start.timestamp = 0;
	start.mutex = &start_mutex;
	prepare_surveillance_data(&panopticon_data, log_arr, philo_args, &start);
	construct_paradigm(episteme, &log_mutex, &log_index, log_arr, philosophers, philo_args, forkexes, &start);
	log_index = 0;
	panopticon_data.log_index = &log_index;
	panopticon_data.log_mutex = &log_mutex;
	run_threads(episteme, &panopticon_data, philo_args, &start);
	return (0);
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

	if (argc != 5 && argc != 6)
		return (philo_exit(wrong_argc));
	err_check = set_philo_args(&philo_args,	argv);
	if (err_check != success)
		return (philo_exit(err_check));
	instantiate_subjects_and_objects(philo_args, philosophers, forkexes);
	prepare_simulation(philo_args, philosophers, forkexes);
	philo_exit(success);
}
