/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                         :+:    :+:           */
/*                                                    +:+ +:+         +:+     */
/*   By: avaliull <avaliull@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 15:27:38 by avaliull          #+#    #+#             */
/*   Updated: 2024/12/17 13:09:23 by avaliull       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

void	gnl_bzero(char **mem, ssize_t size)
{
	while (size--)
		(*mem)[size] = '\0';
}

void	set_str(const char *src, char *dest, ssize_t size)
{
	dest[size] = '\0';
	while (size--)
		dest[size] = src[size];
}

char	*gnl_join(char *str1, char *str2, ssize_t len_1, ssize_t len_2)
{
	ssize_t	new_len;
	char	*new_str;

	new_len = len_1 + len_2;
	new_str = malloc(new_len + 1);
	if (!new_str)
	{
		free(str1);
		return (NULL);
	}
	set_str(str1, new_str, len_1);
	set_str(str2, new_str + len_1, len_2);
	free(str1);
	return (new_str);
}

void	trim_buff(char **buffer, ssize_t last_char_index, ssize_t max_len)
{
	ssize_t	i;
	ssize_t	new_start_index;

	new_start_index = last_char_index + 1;
	i = 0;
	while (i < max_len - new_start_index)
	{
		(*buffer)[i] = (*buffer)[new_start_index + i];
		i++;
	}
	while (i < BUFFER_SIZE)
	{
		(*buffer)[i] = '\0';
		i++;
	}
}
