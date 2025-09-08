/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   ft_free_2d_arr.c                                  :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/05/21 17:19:35 by avaliull     #+#    #+#                  */
/*   Updated: 2025/05/21 17:22:19 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	free_2d_arr(void **arr)
{
	int	i;

	if (arr)
	{
		i = -1;
		while (arr[++i])
		{
			free(arr[i]);
			arr[i] = NULL;
		}
		free(arr);
		arr = NULL;
	}
}
