/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   ft_isupper.c                                      :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/05/21 17:20:50 by avaliull     #+#    #+#                  */
/*   Updated: 2025/05/21 17:21:24 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>

int	ft_isupper(int c)
{
	unsigned char	u_c;

	u_c = (unsigned char) c;
	if (u_c >= 'A' && u_c <= 'Z')
		return (true);
	return (false);
}
