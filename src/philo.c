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

#define NO_LIMIT -1

t_philo_errno	set_philo_args(
	t_philo_args *args,
	char **argv
)
{
	args->philo_count = philo_atoi((unsigned char *) argv[1]);
	args->time_to_die = philo_atoi((unsigned char *) argv[2]);
	args->time_to_eat = philo_atoi((unsigned char *) argv[3]);
	args->time_to_sleep = philo_atoi((unsigned char *) argv[4]);
	if (args->time_to_die <= 0 || args->time_to_eat <= 0
		|| args->time_to_sleep <= 0)
	 	return (invalid_argument);
	//args->time_to_die *= 1000;
	args->time_to_eat *= 1000;
	args->time_to_sleep *= 1000;
	if (argv[5])
	{
		args->meal_count = philo_atoi((unsigned char *) argv[5]);
		if (args->meal_count <= 0)
	 		return (invalid_argument);
	}
	else
		args->meal_count = NO_LIMIT;
	return (success);
}

// defines for forks
// defines for philos

// philosophers and forks are enums (or just ints with a custom value?)
// forks can be USED or UNUSED
// philisophers will have values based on their current action
// both are just an array
// philosophers look only at fork that is same index and index + 1
// they always grab their own index if available
// they grab index + 1 from left to right
// obviously it wraps so last_philo + 1 = fork[0];
// THINKING process continuously checks if forks are used or not
// EATING process simply usleep() while keeping their 2 forks atomic
// SLEEPING just usleep() without doing anything else

unsigned long	get_timestamp_in_ms(
	unsigned long	start_timestamp
)
{
	struct timeval	current_time;
	unsigned long	new_timestamp;

	gettimeofday(&current_time, NULL);
	new_timestamp = current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
	new_timestamp -= start_timestamp;
	return (new_timestamp);
}

unsigned long	get_start_timestamp(
	void
)
{
	struct timeval	timestamp;
	unsigned long	start_timestamp;

	gettimeofday(&timestamp, NULL);
	start_timestamp = timestamp.tv_sec * 1000 + timestamp.tv_usec / 1000;
	return (start_timestamp);
}

void	philo_think(
	t_thread_data *episteme
)
{
	unsigned long	new_timestamp;

	*episteme->philo = THINKING;
	new_timestamp = get_timestamp_in_ms(episteme->start_timestamp);
	printf("%lu philosopher %d is thinking\n",
		new_timestamp, episteme->philo_index + 1);
}

//optimize all of this
void	philo_sleep(
	int	philo_index,
	t_philo	*philo,
	unsigned long start_timestamp,
	unsigned long time_to_sleep
)
{
	unsigned long	sleep_start;
	unsigned long	time_slept;
	const unsigned long	time_to_sleep_in_ms = time_to_sleep / 1000;
	const unsigned long	time_to_sleep_tenth = time_to_sleep / 100;

	*philo = SLEEPING;
	sleep_start = get_timestamp_in_ms(start_timestamp);
	time_slept = get_timestamp_in_ms(start_timestamp) - sleep_start;
	printf("%lu philosopher %d is sleeping\n", sleep_start, philo_index + 1);
	usleep(time_to_sleep / 2);
	//usleep(time_to_sleep_tenth * 9);
	while (time_slept < time_to_sleep_in_ms)
	{
		usleep(50);
		time_slept = get_timestamp_in_ms(start_timestamp) - sleep_start;
	}
}

void	philo_eat(
	unsigned long *last_eaten,
	int	philo_index,
	int	time_to_eat,
	unsigned long start_timestamp
)
{
	unsigned long	new_timestamp;
	unsigned long	time_eaten;
	// timestamping should be on a separate thread?
	// maybe not
	const unsigned long	time_to_eat_in_ms = time_to_eat / 1000;
	const unsigned long	time_to_eat_tenth = time_to_eat / 100;

	*last_eaten = get_timestamp_in_ms(start_timestamp);
	time_eaten = 0;
	printf("%lu philosopher %d is eating\n", *last_eaten, philo_index + 1);
	usleep(time_to_eat / 2);
	//usleep(time_to_eat_tenth * 9);
	while (time_eaten < time_to_eat_in_ms)
	{
		usleep(50);
		time_eaten = get_timestamp_in_ms(start_timestamp) - *last_eaten;
	}
	return ;
}

void	take_a_fork(
	t_thread_data *episteme,
	t_forkex *forkex,
	int *forks_held
)
{
	unsigned long	cur_timestamp;

	if (forkex->fork == UNUSED
			|| (episteme->philo_index % 2 == 0 && forkex->fork == NEVER_USED))
	{
		forkex->fork = USED;
		(*forks_held)++;
		pthread_mutex_unlock(&forkex->mutex);
		cur_timestamp = get_timestamp_in_ms(episteme->start_timestamp);
		printf("%lu philosopher %d took a fork\n",
			cur_timestamp, episteme->philo_index + 1);
	}
	else
		pthread_mutex_unlock(&forkex->mutex);
}

void	find_free_forks(
	t_thread_data *episteme,
	int	*forks_held
)
{
	unsigned long	cur_timestamp;

	cur_timestamp = get_timestamp_in_ms(episteme->start_timestamp);
	pthread_mutex_lock(&episteme->left_forkex->mutex);
	take_a_fork(episteme, episteme->left_forkex, forks_held);
	pthread_mutex_lock(&episteme->right_forkex->mutex);
	take_a_fork(episteme, episteme->right_forkex, forks_held);
}

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
			philo_eat(&last_eaten, episteme->philo_index,
				episteme->philo_args.time_to_eat, episteme->start_timestamp);
			// replace wit hjsut passign episteme
			pthread_mutex_lock(&episteme->left_forkex->mutex);
			episteme->left_forkex->fork = UNUSED;
			pthread_mutex_unlock(&episteme->left_forkex->mutex);
			pthread_mutex_lock(&episteme->right_forkex->mutex);
			episteme->right_forkex->fork = UNUSED;
			pthread_mutex_unlock(&episteme->right_forkex->mutex);
			forks_held = 0;
			if (episteme->philo_args.meal_count != NO_LIMIT)
				times_eaten++;
			philo_sleep(episteme->philo_index, episteme->philo,
			   episteme->start_timestamp, episteme->philo_args.time_to_sleep);
			// replace wit hjsut passign episteme
		}	
	}
	return (0);
}

int	construct_paradigm(
	t_thread_data *episteme,
	t_philo *philosophers,
	t_philo_args philo_args,
	t_forkex *forkexes
)
{
	int	i;

	i = -1;
	while (++i < philo_args.philo_count)
	{
		episteme[i].philo = &philosophers[i];
		episteme[i].philo_args = philo_args;
		episteme[i].philo_args = philo_args;
		episteme[i].left_forkex = &forkexes[i];
		if (i != philo_args.philo_count - 1)
			episteme[i].right_forkex = &forkexes[i + 1];
		else
			episteme[i].right_forkex = forkexes;
		episteme[i].philo_index = i;
		episteme[i].start_timestamp = get_start_timestamp(); // this comes later
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
	pthread_t		*philo_threads;
	pthread_t		panopticon_thread;
	t_thread_data	*episteme;
	unsigned int	i;
	unsigned long	start_timestamp;

	start_timestamp = 0;
	philo_threads = philo_calloc(philo_args.philo_count, sizeof(pthread_t));
	episteme = philo_calloc(philo_args.philo_count, sizeof(t_thread_data));
	if (!philo_threads || !episteme)
		return (-1);
	construct_paradigm(episteme, philosophers, philo_args, forkexes);
	pthread_create(&panopticon_thread, NULL, panopticon, NULL);
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

int	instantiate_subjects_and_objects(
	t_philo_args philo_args,
	t_philo **philosophers,
	t_forkex **forkexes
)
{
	int	i;

	*philosophers = philo_calloc(philo_args.philo_count, sizeof(t_philo));
	*forkexes = philo_calloc(philo_args.philo_count, sizeof(t_forkex));
	if (!*philosophers || !*forkexes)
	{
		printf("malloc error!\n");
		return (1);
	}
	i = -1;
	while (++i < philo_args.philo_count)
	{
		(*forkexes)[i].fork = -1;
		pthread_mutex_init(&(*forkexes)[i].mutex, NULL);
		(*philosophers)[i] = -1;
	}
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
