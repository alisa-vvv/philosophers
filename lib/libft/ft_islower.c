/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   ft_islower.c                                      :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/05/21 17:20:21 by avaliull     #+#    #+#                  */
/*   Updated: 2025/05/21 17:23:23 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>

int	ft_islower(int c)
{
	unsigned char	u_c;

	u_c = (unsigned char) c;
	if (u_c >= 'a' && u_c <= 'z')
		return (true);
	return (false);
}
