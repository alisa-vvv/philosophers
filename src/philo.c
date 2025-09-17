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

	pthread_mutex_lock(episteme->start->mutex);
	episteme->start_timestamp = episteme->start->timestamp;
	pthread_mutex_unlock(episteme->start->mutex);
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
			log_action(episteme->philo_index, MSG_DEAD, episteme->msg_info, cur_timestamp);
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
			philo_sleep(episteme, episteme->philo_index);
		}	
	}
	return (0);
}

void	*panopticon(
	void *data
)
{
	t_panopticon_data	*const panopticon_data = (t_panopticon_data *) data;
	int					i;
	int					first_free_index;
	char				message_buffer[4096];

	printf("PANOPTICON.\n");
	first_free_index = 0;
	pthread_mutex_lock(panopticon_data->start->mutex);
	if (panopticon_data->start->ready == true)
		panopticon_data->start_timestamp = panopticon_data->start->timestamp;
	pthread_mutex_unlock(panopticon_data->start->mutex);

	t_msg_type		msg_type_local;
	unsigned long	timestamp_local;
	int				philo_index_local;

	i = 0;
	while (1)
	{
		usleep(500);
		pthread_mutex_lock(&panopticon_data->msg_info->first_free_index_mutex);
		first_free_index = panopticon_data->msg_info->first_free_index;
		pthread_mutex_unlock(&panopticon_data->msg_info->first_free_index_mutex);
		while (i < first_free_index)
		{
			pthread_mutex_lock(&panopticon_data->msg_info->msg_type_mutex);
			msg_type_local = panopticon_data->msg_info->msg_type[i];
			pthread_mutex_unlock(&panopticon_data->msg_info->msg_type_mutex);
			pthread_mutex_lock(&panopticon_data->msg_info->timestamp_mutex);
			timestamp_local = panopticon_data->msg_info->timestamp[i];
			pthread_mutex_unlock(&panopticon_data->msg_info->timestamp_mutex);
			pthread_mutex_lock(&panopticon_data->msg_info->philo_index_mutex);
			philo_index_local = panopticon_data->msg_info->philo_index[i];
			pthread_mutex_unlock(&panopticon_data->msg_info->philo_index_mutex);
			if (msg_type_local == MSG_DEAD)
			{
				printf("%lu philosopher %d fucking died\n", timestamp_local, philo_index_local + 1);
				return (NULL);
			}	
			else if (msg_type_local == MSG_THINK)
				printf("%lu philosopher %d is thinking\n", timestamp_local, philo_index_local + 1);
			else if (msg_type_local == MSG_FORK)
				printf("%lu philosopher %d took a fork\n", timestamp_local, philo_index_local + 1);
			else if (msg_type_local == MSG_EAT)
				printf("%lu philosopher %d is eating\n", timestamp_local, philo_index_local + 1);
			else if (msg_type_local == MSG_SLEEP)
				printf("%lu philosopher %d is sleeping\n", timestamp_local, philo_index_local + 1);
			i++;
		}
	}
	return (NULL);
}

void	prepare_surveillance_data(
	t_panopticon_data *panopticon_data,
	t_msg_info *msg_info,
	t_philo_args philo_args,
	t_start *start
)
{
	memset(msg_info->msg_type, 0, 1024);
	pthread_mutex_init(&msg_info->msg_type_mutex, NULL);
	memset(msg_info->timestamp, 0, 1024);
	pthread_mutex_init(&msg_info->timestamp_mutex, NULL);
	memset(msg_info->philo_index, 0, 1024);
	pthread_mutex_init(&msg_info->philo_index_mutex, NULL);
	msg_info->first_free_index = 0;
	pthread_mutex_init(&msg_info->first_free_index_mutex, NULL);
	panopticon_data->msg_info = msg_info;
	panopticon_data->philo_count = philo_args.philo_count;
	panopticon_data->meal_count = philo_args.meal_count;
	panopticon_data->start = start;
}

int	prepare_simulation(
	t_philo_args philo_args,
	t_philo *philosophers,
	t_forkex *forkexes
)
{
	t_start				start;
	pthread_mutex_t		start_mutex;
	pthread_t			*philo_threads;
	pthread_t			panopticon_thread;
	t_thread_data		*episteme;
	t_msg_info			msg_info;
	t_panopticon_data	panopticon_data;
	unsigned int		i;

	pthread_mutex_init(&start_mutex, NULL);
	start.ready = false;
	start.timestamp = 0;
	start.mutex = &start_mutex;
	philo_threads = philo_calloc(philo_args.philo_count, sizeof(pthread_t));
	episteme = philo_calloc(philo_args.philo_count, sizeof(t_thread_data));
	if (!philo_threads || !episteme)
		return (-1);
	prepare_surveillance_data(&panopticon_data, &msg_info, philo_args, &start);
	construct_paradigm(episteme, &msg_info, philosophers, philo_args, forkexes, &start);
	pthread_mutex_lock(start.mutex);
	pthread_create(&panopticon_thread, NULL, panopticon, &panopticon_data);
	i = -1;
	while (++i < philo_args.philo_count)
		pthread_create(&philo_threads[i], NULL, praxis, &episteme[i]);
	i = -1;
	start.ready = true;
	start.timestamp = get_start_timestamp();
	pthread_mutex_unlock(start.mutex);
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
