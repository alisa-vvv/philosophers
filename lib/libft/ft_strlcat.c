/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avaliull <avaliull@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 18:51:37 by avaliull          #+#    #+#             */
/*   Updated: 2024/10/16 15:30:40 by avaliull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	srclen;
	size_t	dstlen;
	size_t	catsize;

	dstlen = ft_strlen(dst);
	srclen = ft_strlen(src);
	if (dstlen >= size)
		return (size + srclen);
	catsize = size - dstlen - 1;
	if (srclen < catsize)
		catsize = srclen;
	ft_memcpy(dst + dstlen, src, catsize);
	*(dst + dstlen + catsize) = '\0';
	return (dstlen + srclen);
}
