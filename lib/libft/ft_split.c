/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avaliull <avaliull@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 17:38:17 by avaliull          #+#    #+#             */
/*   Updated: 2024/10/19 16:06:09 by avaliull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static size_t	wcounter(char const *s, char c)
{
	size_t	count;

	count = 0;
	while (*s)
	{
		if (*s == c)
			s++;
		else
		{
			count += 1;
			while (*s && *s != c)
				s++;
		}
	}
	return (count);
}

static char	**memallocer(char const *s, char *c)
{
	size_t	wcount;
	char	**splitted_s;

	if (!s)
		return (NULL);
	wcount = wcounter(s, *c);
	splitted_s = (char **) malloc((wcount + 1) * sizeof (char *));
	if (!splitted_s)
		return (NULL);
	splitted_s[wcount] = NULL;
	return (splitted_s);
}

static const char	*wordallocer(char **arr, char const *s, char c, size_t i)
{
	size_t	symcount;

	symcount = 0;
	while (*s && *s != c)
	{
		s++;
		symcount++;
	}
	arr[i] = (char *) malloc((symcount + 1) * sizeof(char));
	if (!arr[i])
		return (NULL);
	ft_memcpy(arr[i], s - symcount, symcount);
	arr[i][symcount] = '\0';
	return (s);
}

char	**unalloc(char **splitted_s, size_t i)
{
	while (i > 0)
	{
		free (splitted_s[i]);
		i--;
	}
	free (splitted_s[0]);
	free (splitted_s);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	size_t	i;
	char	**splitted_s;

	splitted_s = memallocer(s, &c);
	if (!splitted_s)
		return (NULL);
	i = 0;
	while (*s)
	{
		if (*s == c)
			s++;
		else
		{
			s = wordallocer(splitted_s, s, c, i);
			if (!s)
				return (unalloc(splitted_s, i));
			i++;
		}
	}
	return (splitted_s);
}
