/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                     :+:    :+:           */
/*                                                    +:+ +:+         +:+     */
/*   By: avaliull <avaliull@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 11:32:57 by avaliull          #+#    #+#             */
/*   Updated: 2024/12/15 13:47:17 by avaliull       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

static inline void	ignore_result(ssize_t unused_result)
{
	(void) unused_result;
}

void	ft_putchar_fd(char c, int fd)
{
	ignore_result(write(fd, &c, 1));
}
