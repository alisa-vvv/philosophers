/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                               :+:    :+:           */
/*                                                    +:+ +:+         +:+     */
/*   By: avaliull <avaliull@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 12:37:21 by avaliull          #+#    #+#             */
/*   Updated: 2024/12/13 15:15:45 by avaliull       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static ssize_t	find_nl_index(const char *buffer, int read_len)
{
	int	nl_index;

	nl_index = 0;
	while (nl_index < read_len)
	{
		if (buffer[nl_index] == '\n')
			return (nl_index);
		nl_index++;
	}
	return (nl_index);
}

static char	*eof_or_error(char **buffer, int read_return, char *next_line)
{
	gnl_bzero(buffer, BUFFER_SIZE);
	if (read_return == 0)
		return (next_line);
	free(next_line);
	return (NULL);
}

static char	*read_loop(char **buffer, int fd, char *next_line, ssize_t len)
{
	int	read_return;
	int	nl_i;

	while (1)
	{
		read_return = read(fd, *buffer, BUFFER_SIZE);
		if (read_return <= 0)
			return (eof_or_error(buffer, read_return, next_line));
		nl_i = find_nl_index(*buffer, read_return);
		if (nl_i < read_return)
		{
			next_line = gnl_join(next_line, *buffer, len, nl_i + 1);
			break ;
		}
		next_line = gnl_join(next_line, *buffer, len, read_return);
		if (!next_line)
			return (NULL);
		len += read_return;
	}
	if (nl_i == read_return - 1 && read_return < BUFFER_SIZE)
		gnl_bzero(buffer, BUFFER_SIZE);
	else
		trim_buff(buffer, nl_i, read_return);
	return (next_line);
}

static char	*find_nl(char **buffer, int fd, int buff_len)
{
	char	*next_line;
	int		nl_index;

	nl_index = find_nl_index(*buffer, buff_len);
	if (nl_index == buff_len)
		next_line = malloc(nl_index + 1);
	else
		next_line = malloc(nl_index + 2);
	if (!next_line)
		return (NULL);
	if (nl_index == buff_len)
	{
		set_str(*buffer, next_line, buff_len);
		next_line = read_loop(buffer, fd, next_line, buff_len);
		return (next_line);
	}
	set_str(*buffer, next_line, nl_index + 1);
	if (nl_index == buff_len - 1 && buff_len < BUFFER_SIZE)
		gnl_bzero(buffer, BUFFER_SIZE);
	else
		trim_buff(buffer, nl_index, buff_len);
	return (next_line);
}

char	*get_next_line(int fd)
{
	static char	buffer[1024][BUFFER_SIZE];
	char		*buffer_ptr;
	int			buff_len;

	buff_len = 0;
	if (fd > 1024 || fd < 0)
		return (NULL);
	if (buffer[fd][0] == '\0')
	{
		buff_len = read(fd, buffer[fd], BUFFER_SIZE);
		if (buff_len <= 0)
			return (NULL);
	}
	if (!buff_len)
	{
		while (buffer[fd][buff_len] && buff_len < BUFFER_SIZE)
		{
			if (buffer[fd][buff_len] == '\0')
				break ;
			buff_len++;
		}
	}
	buffer_ptr = &buffer[fd][0];
	return (find_nl(&buffer_ptr, fd, buff_len));
}
