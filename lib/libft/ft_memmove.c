/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                        :+:    :+:           */
/*                                                    +:+ +:+         +:+     */
/*   By: avaliull <avaliull@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 16:50:23 by avaliull          #+#    #+#             */
/*   Updated: 2024/12/15 15:19:48 by avaliull       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*uc_dest;
	unsigned char	*uc_src;

	if (!dest && !src)
		return (0);
	uc_dest = (unsigned char *) dest;
	uc_src = (unsigned char *) src;
	if (dest <= src)
	{
		while (n--)
			*uc_dest++ = *uc_src++;
	}
	else if (dest > src)
	{
		while (n--)
			uc_dest[n] = uc_src[n];
	}
	return (dest);
}
