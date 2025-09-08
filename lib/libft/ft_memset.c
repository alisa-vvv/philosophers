/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                         :+:    :+:           */
/*                                                    +:+ +:+         +:+     */
/*   By: avaliull <avaliull@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 13:13:04 by avaliull          #+#    #+#             */
/*   Updated: 2024/12/15 14:06:28 by avaliull       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*ptr;
	unsigned char	chr;

	chr = (unsigned char) c;
	ptr = (unsigned char *) s;
	while (n--)
		ptr[n] = chr;
	return (s);
}
