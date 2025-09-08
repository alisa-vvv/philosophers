/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                         :+:    :+:           */
/*                                                    +:+ +:+         +:+     */
/*   By: avaliull <avaliull@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 11:25:18 by avaliull          #+#    #+#             */
/*   Updated: 2024/12/15 14:01:56 by avaliull       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*ucs1;
	unsigned char	*ucs2;

	ucs1 = (unsigned char *) s1;
	ucs2 = (unsigned char *) s2;
	while (n--)
	{
		if (*ucs1 != *ucs2)
			return ((*ucs1) - (*ucs2));
		ucs1++;
		ucs2++;
	}
	return (0);
}
