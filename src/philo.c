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

t_philo_errno	set_philo_args(
	t_philo_args *args,
	char **argv
)
{
	args->philo_count = philo_atoi((unsigned char *) argv[1]);
	if (args->philo_count <= 0)
	 	return (invalid_argument);
	args->time_to_die = philo_atoi((unsigned char *) argv[2]);
	if (args->time_to_die <= 0)
	 	return (invalid_argument);
	args->time_to_eat = philo_atoi((unsigned char *) argv[3]);
	if (args->time_to_eat <= 0)
	 	return (invalid_argument);
	args->time_to_eat *= 1000;
	args->time_to_sleep = philo_atoi((unsigned char *) argv[4]);
	if (args->time_to_sleep <= 0)
	 	return (invalid_argument);
	if (argv[5])
	{
		args->meal_count = philo_atoi((unsigned char *) argv[5]);
		if (args->meal_count <= 0)
	 		return (invalid_argument);
	}
	else
		args->meal_count = -2;
	return (success);
}

// defines for forks
#define USED 1
#define UNUSED 0
// defines for philos
#define THINKING 0
#define EATING 1
#define SLEEPING 2
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
	return (new_timestamp);
}

void	philo_state_eating(
	int	philo_index,
	int	time_to_eat,
	unsigned long start_timestamp
)
{
	unsigned long	new_timestamp;

	new_timestamp = get_timestamp_in_ms(start_timestamp);
	printf("%lu philosopher %d is eating\n",
			new_timestamp - start_timestamp, philo_index);
	usleep(time_to_eat);
	new_timestamp = get_timestamp_in_ms(start_timestamp);
	printf("%lu philosopher %d stopped eating\n",
			new_timestamp - start_timestamp, philo_index);
	return ;
}

int	do_philosophy(
	int	i,
	int *forks,
	t_philo_args philo_args,
	unsigned long start_timestamp
)
{
	unsigned long	cur_timestamp;
	// step 1: grab fork
	int times_eaten;

	times_eaten = -1;
	while (++times_eaten < philo_args.meal_count)
	{
		// this loop probably doesn't exist based on how I understand mutex
		// this is THINKING
		while (forks[0] == USED && forks[1] == USED)
		{
			// this is probably running on a separate thread?
			cur_timestamp = get_timestamp_in_ms(start_timestamp);
			if (cur_timestamp - start_timestamp > philo_args.time_to_die)
			{
				printf("philosopher %d fucking died\n", i);
				return (-1);
			}
			continue ;
		}
		// this would be an atomic action
		if (forks[0] == UNUSED && forks[1] == UNUSED)
		{
			//lock muteces
			forks[0] = USED;
			forks[1] = USED;
			philo_state_eating(i, philo_args.time_to_eat, start_timestamp);
			//unlock muteces
			forks[0] = UNUSED;
			forks[1] = UNUSED;
		}
		usleep(philo_args.time_to_sleep);
	}
	return (0);
}

int	philosophy_praxis(
	int *forks,
	int *philosophers,
	t_philo_args philo_args,
	unsigned long start_timestamp
)
{
	unsigned int	i;

	i = -1;
	while (++i < philo_args.philo_count)
	{
		do_philosophy(i, forks, philo_args, start_timestamp);
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
	int				*philosophers;
	int				*forks;
	struct timeval	timestamp;
	unsigned long	start_timestamp;

	if (argc != 5 && argc != 6)
		return (philo_exit(wrong_argc));
	err_check = set_philo_args(&philo_args,	argv);
	if (err_check != success)
		return (philo_exit(err_check));
	philosophers = philo_calloc(philo_args.philo_count, sizeof(int));
	forks = philo_calloc(philo_args.philo_count, sizeof(int));
	if (!philosophers || !forks)
	{
		printf("malloc error!\n");
		return (1);
	}
	test_print_args(&philo_args);
	gettimeofday(&timestamp, NULL);
	start_timestamp = timestamp.tv_sec * 1000 + timestamp.tv_usec / 1000;
	philosophy_praxis(forks, philosophers, philo_args, start_timestamp);
	free(philosophers);
	free(forks);
	philo_exit(success);
}
