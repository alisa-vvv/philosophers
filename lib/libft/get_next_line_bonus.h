/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                               :+:    :+:           */
/*                                                    +:+ +:+         +:+     */
/*   By: avaliull <avaliull@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 12:59:44 by avaliull          #+#    #+#             */
/*   Updated: 2024/12/14 12:46:15 by avaliull       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <unistd.h>
# include <stdlib.h>

/*	These includes are for testing only	*/
//#include <stdio.h>

# if defined(BUFFER_SIZE)
#  if BUFFER_SIZE > 8300000 || BUFFER_SIZE < 1
#   undef BUFFER_SIZE
#   define BUFFER_SIZE 42
#  endif
# else
#  define BUFFER_SIZE 42
# endif

char	*get_next_line(int fd);
void	set_str(const char *src, char *dest, ssize_t size);
char	*gnl_join(char *str1, char *str2, ssize_t len_1, ssize_t len_2);
void	trim_buff(char **buffer, ssize_t last_char_index, ssize_t max_len);
void	gnl_bzero(char **mem, ssize_t size);

#endif
