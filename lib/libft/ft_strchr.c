/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                         :+:    :+:           */
/*                                                    +:+ +:+         +:+     */
/*   By: avaliull <avaliull@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:19:53 by avaliull          #+#    #+#             */
/*   Updated: 2024/12/15 14:08:47 by avaliull       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

char	*ft_strchr(const char *s, int c)
{
	char	*ptr_s;
	char	chr;

	chr = (char) c;
	ptr_s = (char *) s;
	while (*ptr_s)
	{
		if (*ptr_s == chr)
			return (ptr_s);
		ptr_s++;
	}
	if (chr == '\0')
		return (ptr_s);
	return (NULL);
}
