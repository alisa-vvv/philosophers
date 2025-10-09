// ************************************************************************** //
//                                                                            //
//                                                       ::::::::             //
//   praxis.c                                          :+:    :+:             //
//                                                    +:+                     //
//   By: avaliull <avaliull@student.codam.nl>        +#+                      //
//                                                  +#+                       //
//   Created: 2025/10/08 17:30:34 by avaliull     #+#    #+#                  //
//   Updated: 2025/10/08 17:39:23 by avaliull     ########   odam.nl          //
//                                                                            //
// ************************************************************************** //

#include "philo.h"

static int	eat_and_sleep(
	t_thread_data *const episteme,
	unsigned long *const last_eaten,
	int *const times_eaten,
	int *const forks_held
)
{
	int	stop;

  	*episteme->philo = EATING;
  	stop = philo_eat(episteme, last_eaten, episteme->philo_i, forks_held);
  	if (stop != success)
  		return (stop);
  	if (episteme->meal_count != NO_LIMIT)
  		(*times_eaten)++;
  	stop = philo_sleep(episteme, last_eaten, episteme->philo_i);
  	if (stop == death)
  		return (death);
	return (success);
}

static int	routine(// go throug hthis whole thing
	t_thread_data *const episteme,
	unsigned long *const last_eaten,
	int *const times_eaten,
	int *const forks_held
)
{
	int				stop;
	unsigned long	timestamp;

	philo_think(episteme); // this
	stop = find_free_forks(episteme, forks_held);
	if (stop != success)
		return (stop); // WHAT HAPPENS ON RETURN?
	stop = check_if_dead(episteme, last_eaten); // this and everything below
	if (stop != success)
		return (death);
	if (*forks_held == 2)
	{
		stop = eat_and_sleep(episteme, last_eaten, times_eaten, forks_held); //inside here
		if (stop != success)
			return (death);
	}
	if (check_if_dead(episteme, last_eaten) == death)
		return (death);
	return (success);
}

static int	check_simulation_end(
	t_thread_data	*const episteme
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

static int	wait_for_start(
	t_thread_data *const episteme
)
{
	if (pthread_mutex_lock(episteme->start->mutex) != 0)
		return (1);
	if (episteme->start->run_simulation == false)
		return (1);
	episteme->start_stamp = episteme->start->timestamp;
	if (pthread_mutex_unlock(episteme->start->mutex) != 0)
		return (1);
	return (0);
}

void	*praxis(
	void *data
)
{
	t_thread_data *const	episteme = (t_thread_data *) data;
	unsigned long			last_eaten;
	int						times_eaten;
	int						total_meals;
	int						forks_held;

	last_eaten = 0;
	times_eaten = 0;
	forks_held = 0;
	if (wait_for_start(episteme) != 0)
		return (NULL);
	if (episteme->meal_count >= 0)
		total_meals = episteme->meal_count;
	else if (episteme->meal_count == NO_LIMIT)
		total_meals = 1;
	while (times_eaten < total_meals)
	{
		if (check_simulation_end(episteme) == 1)
			return (NULL);
		if (routine(episteme, &last_eaten, &times_eaten, &forks_held) == 1) // add here some error management, no 1 returns, should all be descriptive enums
			return (NULL);
	}
	return (NULL);
}
