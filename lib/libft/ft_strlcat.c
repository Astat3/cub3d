/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roazouan <roazouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 17:05:03 by roazouan          #+#    #+#             */
/*   Updated: 2025/11/09 17:15:24 by roazouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	src_len;
	size_t	i;

	src_len = ft_strlen((char *)dst);
	i = 0;
	if (size <= src_len)
		return (ft_strlen((char *)src) + size);
	while (src[i] && (src_len + i) < (size - 1))
	{
		dst[src_len + i] = src[i];
		i++;
	}
	dst[src_len + i] = 0;
	return (ft_strlen((char *)src) + src_len);
}
