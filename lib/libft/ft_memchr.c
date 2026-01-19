/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roazouan <roazouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 17:03:06 by roazouan          #+#    #+#             */
/*   Updated: 2025/11/09 17:06:44 by roazouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*str;
	size_t			i;
	unsigned char	target;

	i = 0;
	target = (unsigned char)c;
	str = (unsigned char *)s;
	while (i < n)
	{
		if (str[i] == target)
			return ((void *)&str[i]);
		i++;
	}
	return (NULL);
}
