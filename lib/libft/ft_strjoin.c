/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roazouan <roazouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 17:04:58 by roazouan          #+#    #+#             */
/*   Updated: 2026/01/20 20:15:17 by roazouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(const char *s1, const char *s2)
{
    char	*t;
    int		i;
    int		j;

    if (!s1 && !s2)
        return (NULL);
    if (!s1)
        return (ft_strdup(s2));
    if (!s2)
        return (ft_strdup(s1));
    t = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
    if (!t)
        return (NULL);
    i = -1;
    while (s1[++i])
        t[i] = s1[i];
    j = 0;
    while (s2[j])
        t[i++] = s2[j++];
    t[i] = '\0';
    return (t);
}

// int main()
// {
//     printf("%s\n", ft_strjoin("slaut", "bonjour"));
//     return (0);
// }
