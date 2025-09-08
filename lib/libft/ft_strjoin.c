/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avaliull <avaliull@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 14:39:02 by avaliull          #+#    #+#             */
/*   Updated: 2024/10/18 18:05:25 by avaliull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*cateds;
	size_t	len;
	size_t	s1_len;

	s1_len = ft_strlen(s1);
	len = s1_len + ft_strlen(s2) + 1;
	cateds = (char *) malloc (len * sizeof(char));
	if (!cateds)
		return (NULL);
	ft_memcpy(cateds, s1, s1_len);
	ft_memcpy(cateds + s1_len, s2, len - s1_len);
	*(cateds + len - 1) = '\0';
	return (cateds);
}
