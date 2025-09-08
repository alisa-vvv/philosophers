// ************************************************************************** //
//                                                                            //
//                                                       ::::::::             //
//   ft_safe_free.c                                    :+:    :+:             //
//                                                    +:+                     //
//   By: avaliull <avaliull@student.codam.nl>        +#+                      //
//                                                  +#+                       //
//   Created: 2025/08/14 16:05:11 by avaliull     #+#    #+#                  //
//   Updated: 2025/08/14 16:46:51 by avaliull     ########   odam.nl          //
//                                                                            //
// ************************************************************************** //

#include <stdlib.h>

void	ft_safe_free(unsigned char **mem)
{
	if (mem)
	{
		if (*mem)
			free(*mem);
		*mem = NULL;
	}
}
