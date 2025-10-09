#include "philo.h"
#include <string.h>
#include <pthread.h> 

static void	prepare_surveillance_data(
	t_panopticon_data *const panopticon_data,
	t_philo_args philo_args,
	t_start *const start
)
{
	memset(panopticon_data->meals_eaten, 0, PHILO_BUF_MAX);
	panopticon_data->philo_count = philo_args.philo_count;
	panopticon_data->meal_count = philo_args.meal_count;
	panopticon_data->philos_sated = 0;
	panopticon_data->start = start;
}

static int	log_setup_sim_run(
	t_panopticon_data *const panopticon_data,
	t_thread_data *const episteme,
	t_philo_args philo_args,
	t_start *const start
)
{
	unsigned long	log_arr[LOG_BUF_MAX];
	unsigned long	log_index;
	pthread_mutex_t	log_mutex;
	int				i;

	memset(log_arr, 0, LOG_BUF_MAX);
	log_index = 0;
	if (pthread_mutex_init(&log_mutex, NULL) != 0)
		return (mutex_init_fail);
	panopticon_data->log_index = &log_index;
	panopticon_data->log_mutex = &log_mutex;
	panopticon_data->log_arr = log_arr;
	i = -1;
	while (++i < philo_args.philo_count)
	{
		episteme[i].log_index = &log_index;
		episteme[i].log_mutex = &log_mutex;
		episteme[i].log_arr = log_arr;
	}
	return (run_threads(episteme, panopticon_data, philo_args, start));
}

static void	construct_paradigm(
	t_thread_data *const episteme,
	t_philo_args philo_args,
	t_philo *const philosophers,
	t_forkex *const forkexes
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
			episteme[i].right_forkex = forkexes;
		episteme[i].philo_i = i;
	}
}

static int	prepare_and_run_simulation(
	t_philo_args philo_args,
	t_philo *const philosophers,
	t_forkex *const forkexes
)
{
	t_start				start;
	pthread_mutex_t		start_mutex;
	t_thread_data		episteme[PHILO_BUF_MAX];
	t_panopticon_data	panopticon_data;
	int					i;

	if (pthread_mutex_init(&start_mutex, NULL) != 0)
		return (mutex_init_fail);
	start.run_simulation = false;
	start.timestamp = 0;
	start.mutex = &start_mutex;
	i = -1;
	while (++i < philo_args.philo_count)
		episteme[i].start = &start;
	prepare_surveillance_data(&panopticon_data, philo_args, &start);
	construct_paradigm(episteme, philo_args, philosophers, forkexes);
	return (log_setup_sim_run(&panopticon_data, episteme, philo_args, &start));
}

int	main(
	const int argc,
	const char *const *const argv
)
{
	t_philo_errno	err;
	t_philo_args	philo_args;
	t_philo			philosophers[PHILO_BUF_MAX];
	t_forkex		forkexes[PHILO_BUF_MAX];

	if (argc != 5 && argc != 6)
		return (philo_exit(wrong_argc));
	err = set_philo_args(&philo_args, argv);
	if (err != success)
		return (philo_exit(err));
	err = instantiate_subjects_and_objects(philo_args, philosophers, forkexes);
	if (err != success)
		philo_exit(mutex_init_fail);
	err = prepare_and_run_simulation(philo_args, philosophers, forkexes);
	if (err != success)
		philo_exit(err);
	philo_exit(success);
}
