/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avaliull <avaliull@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 18:27:31 by avaliull          #+#    #+#             */
/*   Updated: 2024/10/16 11:29:04 by avaliull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	ind;

	if (!s || !f)
		return ;
	ind = 0;
	while (s[ind])
	{
		f(ind, &s[ind]);
		ind++;
	}
}
