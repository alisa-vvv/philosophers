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
	int	status;

  	*episteme->philo = EATING;
  	status = philo_eat(episteme, last_eaten, episteme->philo_i, forks_held);
  	if (status != success)
  		return (status);
  	if (episteme->meal_count != NO_LIMIT)
  		(*times_eaten)++;
  	status = philo_sleep(episteme, last_eaten, episteme->philo_i);
	return (status);
}

static int	routine(// go throug hthis whole thing
	t_thread_data *const episteme,
	unsigned long *const last_eaten,
	int *const times_eaten,
	int *const forks_held
)
{
	int				status;
	unsigned long	timestamp;

	status = philo_think(episteme);
	if (status != success)
		return (status); // WHAT HAPPENS ON RETURN?
	status = find_free_forks(episteme, forks_held);
	if (status != success)
		return (status); // WHAT HAPPENS ON RETURN?
	status = check_if_dead(episteme, last_eaten);
	if (status != success)
		return (death);
	if (*forks_held == 2)
	{
		status = eat_and_sleep(episteme, last_eaten, times_eaten, forks_held);
		if (status != success)
			return (status);
	}
	status = check_if_dead(episteme, last_eaten);
	return (status);
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

static void *check_error(
)
{
	return (NULL);
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
		if (routine(episteme, &last_eaten, &times_eaten, &forks_held) != 0)
			return (check_error()); // here
	}
	return (NULL);
}
