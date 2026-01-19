/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roazouan <roazouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 17:05:40 by roazouan          #+#    #+#             */
/*   Updated: 2025/11/09 17:06:44 by roazouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// int is_set(const char c, const char *set)
// {
//     int i;

//     i = 0;
//     while (set[i])
//     {
//         if (c == set[i])
//             return (1);
//         i++;
//     }
//     return (0);
// }

// int count_set(const char *str, const char *set)
// {
//     int i;
//     int c;

//     i = 0;
//     c = 0;
//     while(str[i])
//     {
//         if (is_set(str[i], set) == 1)
//             c++;
//         i++;
//     }
//     return (c);
// }

// char *ft_strtrim(const char *s1, const char *set)
// {
//     int c;
//     int i;
//     int j;
//     char *res;

//     if (!s1)
//         return (NULL);
//     j = 0;
//     i = -1;
//     c = count_set(s1, set);
//     res = malloc(sizeof(char) * ((ft_strlen((char *)s1) - c) + 1));
//     if (!res)
//         return (NULL);
//     while(s1[++i])
//     {
//         while (is_set(s1[i], set) == 1)
//             i++;
//         res[j++] = s1[i];
//     }
//     res[j] = '\0';
//     return (res);
// }

static int	check_char(char const *charset, char c)
{
	int	i;

	i = 0;
	while (charset[i])
	{
		if (charset[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		i;
	int		start;
	int		end;
	char	*str;

	start = 0;
	i = 0;
	if (!s1 || !set)
		return (NULL);
	end = ft_strlen(s1);
	while (s1[start] && check_char(set, s1[start]))
		start++;
	while (end > start && check_char(set, s1[end - 1]))
		end--;
	str = malloc(sizeof(char) * (end - start) + 1);
	if (!str)
		return (NULL);
	while (s1[start] && start < end)
	{
		str[i] = s1[start];
		i++;
		start++;
	}
	str[i] = 0;
	return (str);
}

// int main()
// {
//     printf("%s\n", ft_strtrim("abcdba", "acb"));
//     return(0);
// }
