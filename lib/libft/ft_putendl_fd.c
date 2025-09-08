/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                     :+:    :+:           */
/*                                                    +:+ +:+         +:+     */
/*   By: avaliull <avaliull@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 12:25:40 by avaliull          #+#    #+#             */
/*   Updated: 2024/12/15 13:48:36 by avaliull       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>

static inline void	ignore_result(ssize_t unused_result)
{
	(void) unused_result;
}

void	ft_putendl_fd(char *s, int fd)
{
	if (!s)
		return ;
	ignore_result(write(fd, s, ft_strlen(s)));
	ignore_result(write(fd, "\n", 1));
}
