#include "libft.h"
#include "philo.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h> // - printf
#include <unistd.h> // - write, usleep. IN HEADER
#include <string.h> // - memset
#include <stdlib.h> // for malloc
#include <sys/time.h> // for gettimeofday
#include <pthread.h> /* for all pthread_ things:
* pthread_create, pthread_detach, pthread_join, pthread_mutex_init,
* pthread_mutex_destroy, pthread_mutex_lock,
* pthread_mutex_unlock
*/

int	routine(
	t_thread_data *episteme,
	unsigned long *last_eaten,
	int *times_eaten,
	int *forks_held
)
{
	int				stop;
	unsigned long	timestamp;

	philo_think(episteme);
	find_free_forks(episteme, forks_held);
	if (*forks_held == 2)
	{
		*episteme->philo = EATING;
		stop = philo_eat(episteme, last_eaten, episteme->philo_index, forks_held);
		if (stop == 1)
			return (1);
		if (episteme->meal_count != NO_LIMIT)
			(*times_eaten)++;
		stop = philo_sleep(episteme, episteme->philo_index);
		if (stop == 1)
			return (1);
	}	
	timestamp = get_timestamp_in_ms(episteme->start_timestamp);
	if (timestamp - *last_eaten > episteme->time_to_die)
	{
		pthread_mutex_lock(episteme->start->mutex);
		episteme->start->run_simulation = false;
		pthread_mutex_unlock(episteme->start->mutex);
		log_action(episteme, episteme->philo_index, MSG_DEAD, timestamp);
		return (1);
	}
	return (0);
}

int	check_simulation_end(
	t_thread_data	*episteme
)
{
	pthread_mutex_lock(episteme->start->mutex);
	if (episteme->start->run_simulation == false)
	{
		pthread_mutex_unlock(episteme->start->mutex);
		return (1);
	}
	pthread_mutex_unlock(episteme->start->mutex);
	return (0);
}

void	*praxis(
	void	*data
)
{
	t_thread_data	*episteme = (t_thread_data *) data;
	unsigned long	last_eaten;
	int				times_eaten;
	int				total_meals;
	int				forks_held;

	last_eaten = 0;
	times_eaten = 0;
	forks_held = 0;
	pthread_mutex_lock(episteme->start->mutex);
	episteme->start_timestamp = episteme->start->timestamp;
	pthread_mutex_unlock(episteme->start->mutex);
	if (episteme->meal_count >= 0)
		total_meals = episteme->meal_count;
	else if (episteme->meal_count == NO_LIMIT)
		total_meals = 1;
	while (times_eaten < total_meals)
	{
		if (check_simulation_end(episteme) == 1)
			return (NULL);
		if (routine(episteme, &last_eaten, &times_eaten, &forks_held) == 1)
			return (NULL);
	}
	return (NULL);
}

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

int	run_threads(
	t_thread_data *episteme,
	t_panopticon_data *panopticon_data,
	t_philo_args philo_args,
	t_start *start
)
{
	pthread_t	panopticon_thread;
	pthread_t	philo_threads[PHILO_BUF_MAX];
	int			i;

	pthread_mutex_lock(start->mutex);
	pthread_create(&panopticon_thread, NULL, panopticon, panopticon_data);
	i = -1;
	while (++i < philo_args.philo_count)
		pthread_create(&philo_threads[i], NULL, praxis, &episteme[i]);
	i = -1;
	usleep(1000);
	start->run_simulation = true;
	start->timestamp = get_start_timestamp();
	pthread_mutex_unlock(start->mutex);
	pthread_join(panopticon_thread, NULL);
	while (++i < philo_args.philo_count)
		pthread_join(philo_threads[i], NULL);
	return (0);
}

int	prepare_simulation(
	t_philo_args philo_args,
	t_philo *philosophers,
	t_forkex *forkexes
)
{
	t_start				start;
	pthread_mutex_t		start_mutex;
	//t_thread_data		*episteme;

	t_thread_data		episteme[PHILO_BUF_MAX];
	t_panopticon_data	panopticon_data;

	unsigned long		log_arr[LOG_BUF_MAX];
	unsigned long		log_index;
	pthread_mutex_t		log_mutex;

	// calloc episteme w/e
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
	//t_philo			*philosophers;
	//t_forkex		*forkexes;
	t_philo			philosophers[PHILO_BUF_MAX];
	t_forkex		forkexes[PHILO_BUF_MAX];

	if (argc != 5 && argc != 6)
		return (philo_exit(wrong_argc));
	err_check = set_philo_args(&philo_args,	argv);
	if (err_check != success)
		return (philo_exit(err_check));
	//philosophers = NULL;
	//forkexes = NULL;
	instantiate_subjects_and_objects(philo_args, philosophers, forkexes);
	TEST_print_args(&philo_args);
	prepare_simulation(philo_args, philosophers, forkexes);
	//free(philosophers);
	//free(forkexes);
	philo_exit(success);
}
