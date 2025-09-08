/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   ft_isspace.c                                      :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/05/21 17:20:30 by avaliull     #+#    #+#                  */
/*   Updated: 2025/05/21 17:21:36 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>

int	ft_isspace(int c)
{
	unsigned char	u_c;

	u_c = (unsigned char) c;
	if ((u_c >= '\t' && u_c <= '\r') || u_c == ' ')
		return (true);
	return (false);
}
