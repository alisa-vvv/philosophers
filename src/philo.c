#include "philo.h"
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
	t_philo_args	*args,
	char			**argv
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

// philosophers and forks are enums
// forks can be USED or UNUSED
// philisophers will have values based on their current action
// fork
int	main(
	int				argc,
	char			**argv
)
{
	t_philo_errno	err_check;
	t_philo_args	philo_args;
	int				*philosophers;
	bool			*forks;

	if (argc != 5 && argc != 6)
		return (philo_exit(wrong_argc));
	err_check = set_philo_args(&philo_args,	argv);
	if (err_check != success)
		return (philo_exit(err_check));
	test_print_args(&philo_args);
	philo_exit(success);
}
