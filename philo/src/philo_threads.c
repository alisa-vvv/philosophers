/* ************************************************************************** */
/*                                                                            */
/*                                                            ::::::::        */
/*   philo_threads.c                                         :+:    :+:       */
/*                                                          +:+               */
/*   By: avaliull <avaliull@student.codam.nl>              +#+                */
/*                                                        +#+                 */
/*   Created: 2025/10/17 13:26:52 by avaliull            #+#    #+#           */
/*   Updated: 2025/10/17 13:44:27 by avaliull            ########   odam.nl   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>

static int	routine(
	t_thread_data *episteme,
	unsigned long *last_eaten,
	int *forks_held
)
{
	if (philo_think(episteme) == 1)
		return (1);
	if (find_free_forks(episteme, forks_held, last_eaten) == 1)
		return (1);
	if (check_if_dead(episteme, last_eaten) == 1)
		return (1);
	if (*forks_held == 2)
	{
		*episteme->philo = EATING;
		if (philo_eat(episteme, last_eaten, forks_held) == 1)
			return (1);
		if (philo_sleep(episteme, last_eaten) == 1)
			return (1);
	}
	if (check_if_dead(episteme, last_eaten) == 1)
		return (1);
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

/* Praxis is the process by which a theory, lesson, or skill is enacted,
 * embodied, realized, applied, or put into practice. "Praxis" may also refer to
 * the act of engaging, applying, exercising, realizing, or practising ideas.
 *
 * Thinking, sleeping and eating spaghetti is praxis.
*/
static void	*praxis(
	void *data
)
{
	t_thread_data *const	episteme = (t_thread_data *) data;
	unsigned long			last_eaten;
	int						forks_held;

	forks_held = 0;
	last_eaten = 0;
	pthread_mutex_lock(episteme->start->mutex);
	episteme->start_timestamp = episteme->start->timestamp;
	pthread_mutex_unlock(episteme->start->mutex);
	while (1)
	{
		if (check_simulation_end(episteme) == 1)
			break ;
		if (routine(episteme, &last_eaten, &forks_held) == 1)
			break ;
	}
	return (NULL);
}

int	run_threads(
	t_thread_data *episteme,
	t_panopticon_data *panopticon_data,
	t_philo_args philo_args,
	t_start *start
)
{
	pthread_t	philo_threads[PHILO_BUF_MAX];
	int			i;

	pthread_mutex_lock(start->mutex);
	start->run_simulation = true;
	start->timestamp = get_start_timestamp();
	panopticon_data->start_timestamp = start->timestamp;
	i = -1;
	while (++i < philo_args.philo_count)
		pthread_create(&philo_threads[i], NULL, praxis, &episteme[i]); // add error case
	pthread_mutex_unlock(start->mutex);
	panopticon(panopticon_data);
	i = -1;
	while (++i < philo_args.philo_count)
		pthread_join(philo_threads[i], NULL); // add error case
	return (0);
}
