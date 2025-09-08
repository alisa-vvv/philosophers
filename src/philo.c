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
#define USED 1;
#define FREE 0;
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

void	philo_state_eating(
	int	philo_index,
	int	time_to_eat,
	int	*left_fork,
	int	*right_fork,
	unsigned long *start_timestamp
)
{
	//stupid stupid stupid stupid stupid stupid stupid stupid stupid stupid stupid stupid stupid stupid 
	struct timeval	current_time;
	unsigned long	new_timestamp;

	gettimeofday(&current_time, NULL);
	new_timestamp = current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
	printf("%lu philosopher %d is eating\n",
			new_timestamp - *start_timestamp, philo_index);
	usleep(time_to_eat - MILLISECOND);
	gettimeofday(&current_time, NULL);
	new_timestamp = current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
	printf("%lu philosopher %d stopped eating\n",
			new_timestamp - *start_timestamp, philo_index);
	return ;
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
	philo_state_eating(0, philo_args.time_to_eat, &forks[0], &forks[1], &start_timestamp);
	free(philosophers);
	free(forks);
	philo_exit(success);
}
