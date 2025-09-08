#include "philo.h"
#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>

// we might be able to get away with jsut one function?
void	philo_state_sleeping(
	int	philo_index,
	int	time_to_sleep
)
{
	printf("%d philosopher %d is sleeping\n", 123, philo_index);
	usleep(time_to_sleep);
	return ;
}

void	philo_state_thinking(
	int	philo_index
)
{
	printf("%d philosopher %d is thinking\n", 123, philo_index);
	// check if fork available?
	return ;
}

