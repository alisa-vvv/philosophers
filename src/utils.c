/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   utils.c                                           :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/05/30 17:54:08 by avaliull     #+#    #+#                  */
/*   Updated: 2025/05/30 17:56:07 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int	philo_putstr_fd(
	int		fd,
	char	*str
)
{
	int	len;

	len = 0;
	while(str[len])
		len++;
	return (write(fd, str, len));
}

void	*philo_calloc(
	size_t nmemb,
	size_t size
)
{
	unsigned char	*memory;
	int				i;

	if (size == 0)
		return (malloc(0));
	if (nmemb > (size_t) - 1 / size)
		return (NULL);
	memory = malloc(nmemb * size);
	if (memory == NULL)
		return (NULL);
	i = -1;
	while (++i < nmemb)
		memory[i] = 0;
	return (memory);
}

int		philo_atoi(
	unsigned char	*str
)
{
	int	i;
	int	n;

	i = 0;
	n = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (i > 10)
			return (-1);
		if (n < 0)
			return (-1);
		n = n * 10 + str[i] - '0';
		i++;
	}
	if (str[i] && (str[i] < '0' || str[i] > '9'))
		return (-1);
	return (n);
}

