/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avaliull <avaliull@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 17:23:25 by avaliull          #+#    #+#             */
/*   Updated: 2024/10/16 16:19:54 by avaliull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	ind;
	char			*resstr;
	unsigned int	len;

	if (!s || !f)
		return (NULL);
	len = ft_strlen(s);
	ind = 0;
	resstr = (char *) malloc((len + 1) * sizeof(char));
	if (!resstr)
		return (NULL);
	while (ind < len)
	{
		resstr[ind] = f(ind, s[ind]);
		ind++;
	}
	resstr[len] = 0;
	return (resstr);
}
