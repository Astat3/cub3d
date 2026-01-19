/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roazouan <roazouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 17:04:45 by roazouan          #+#    #+#             */
/*   Updated: 2025/11/09 17:14:38 by roazouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int		i;
	char	v;

	v = (unsigned char)c;
	i = -1;
	while (s[++i])
		if (s[i] == v)
			return ((char *)&s[i]);
	if (s[i] == v)
		return ((char *)&s[i]);
	return (0);
}
