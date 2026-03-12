/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roazouan <roazouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 17:05:34 by roazouan          #+#    #+#             */
/*   Updated: 2025/11/09 17:06:44 by roazouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int				i;
	unsigned char	target;

	target = (unsigned char)c;
	if (!s)
		return (NULL);
	i = (ft_strlen((char *)s));
	while (i >= 0)
	{
		if ((char)s[i] == target)
			return ((char *)s + i);
		i--;
	}
	return (NULL);
}
