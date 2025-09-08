/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avaliull <avaliull@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 14:57:28 by avaliull          #+#    #+#             */
/*   Updated: 2024/10/19 16:08:38 by avaliull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static	char	*trimright(char const *s, char const *set)
{
	while (*s)
	{
		if (!ft_strchr(set, *s))
			return ((char *)s);
		s++;
	}
	return ((char *) s);
}

static	char	*trimleft(char const *s, char const *set, size_t j, size_t i)
{
	if (s[i] == '\0')
		return ((char *) &s[i]);
	while (j > i)
	{
		if (!ft_strchr(set, s[j]))
			return ((char *) &s[j + 1]);
		j--;
	}
	return ((char *) &s[i + 1]);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*s1start;
	char	*s1end;
	char	*newstr;
	int		newstrlen;
	int		s1len;

	if (!set || !s1)
		return (NULL);
	s1len = ft_strlen(s1);
	s1start = trimright(s1, set);
	s1end = trimleft(s1, set, s1len - 1, s1start - s1);
	newstrlen = (s1end - s1start);
	newstr = (char *) malloc((newstrlen + 1) * sizeof(char));
	if (!newstr)
		return (NULL);
	ft_memcpy(newstr, s1start, newstrlen);
	newstr[newstrlen] = '\0';
	return (newstr);
}
