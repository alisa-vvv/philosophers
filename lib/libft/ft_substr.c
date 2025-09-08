/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avaliull <avaliull@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 13:35:56 by avaliull          #+#    #+#             */
/*   Updated: 2024/10/17 17:57:21 by avaliull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*newstr;
	size_t	lentoend;

	lentoend = 0;
	lentoend += ft_strlen((char *) s);
	if (!lentoend || start > lentoend)
	{
		len = 0;
		start = lentoend;
	}
	else
		lentoend -= start;
	if (len > lentoend)
		len = lentoend;
	newstr = (char *) malloc((len + 1) * sizeof (char));
	if (!newstr)
		return (NULL);
	ft_memcpy(newstr, s + start, len);
	*(newstr + len) = '\0';
	return (newstr);
}
