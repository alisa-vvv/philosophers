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

void	*praxis(
	void	*data
)
{
	t_thread_data	*episteme = (t_thread_data *) data;
	unsigned long	cur_timestamp;
	unsigned long	last_eaten;
	int				times_eaten;
	int				total_meals;
	int				forks_held;

	last_eaten = 0; // this should be a shared variable in episteme
	if (episteme->philo_args.meal_count >= 0)
		total_meals = episteme->philo_args.meal_count;
	else if (episteme->philo_args.meal_count == NO_LIMIT)
		total_meals = 1;
	// meal count should also be handled by manager thread i think
	times_eaten = 0;
	forks_held = 0;
	while (times_eaten < total_meals)
	{
		if (*episteme->philo != THINKING)	
			philo_think(episteme);

		/* this goes to manager thread */
		cur_timestamp = get_timestamp_in_ms(episteme->start_timestamp);
		if (cur_timestamp - last_eaten > episteme->philo_args.time_to_die)
		{
			printf("philosopher %d last eaten: %lu\n", episteme->philo_index + 1, last_eaten);
			printf("%lu philosopher %d fucking died\n", cur_timestamp, episteme->philo_index + 1);
			return (NULL);
		}
		/*	*/
		find_free_forks(episteme, &forks_held);
		if (forks_held == 2)
		{
			*episteme->philo = EATING;
			philo_eat(episteme, &last_eaten, episteme->philo_index, &forks_held);
			if (episteme->philo_args.meal_count != NO_LIMIT)
				times_eaten++;
			philo_sleep(episteme->philo_index, episteme->philo,
			   episteme->start_timestamp, episteme->philo_args.time_to_sleep);
			// replace wit hjsut passign episteme
			// why though
		}	
	}
	return (0);
}

void	*panopticon(
	void *data
)
{
	printf("PANOPTICON.\n");
	return (NULL);
}

int	prepare_simulation(
	t_philo_args philo_args,
	t_philo *philosophers,
	t_forkex *forkexes
)
{
	bool			simulation_started;
	pthread_t		*philo_threads;
	pthread_t		panopticon_thread;
	t_thread_data	*episteme;
	t_thread_data	tracking;
	unsigned int	i;
	unsigned long	start_timestamp;

	simulation_started = false;
	start_timestamp = 0;
	philo_threads = philo_calloc(philo_args.philo_count, sizeof(pthread_t));
	episteme = philo_calloc(philo_args.philo_count, sizeof(t_thread_data));
	if (!philo_threads || !episteme)
		return (-1);
	construct_paradigm(episteme, philosophers, philo_args, forkexes);
	pthread_create(&panopticon_thread, NULL, panopticon, &tracking);
	i = -1;
	while (++i < philo_args.philo_count)
		pthread_create(&philo_threads[i], NULL, praxis, &episteme[i]);
	i = -1;
	while (++i < philo_args.philo_count)
		pthread_join(philo_threads[i], NULL);
	free(episteme);
	free(philo_threads);
	return (0);
}

int	main(
	int argc,
	char **argv
)
{
	t_philo_errno	err_check;
	t_philo_args	philo_args;
	t_philo			*philosophers;
	t_forkex		*forkexes;

	if (argc != 5 && argc != 6)
		return (philo_exit(wrong_argc));
	err_check = set_philo_args(&philo_args,	argv);
	if (err_check != success)
		return (philo_exit(err_check));
	philosophers = NULL;
	forkexes = NULL;
	instantiate_subjects_and_objects(philo_args, &philosophers, &forkexes);
	TEST_print_args(&philo_args);
	prepare_simulation(philo_args, philosophers, forkexes);
	free(philosophers);
	free(forkexes);
	philo_exit(success);
}
