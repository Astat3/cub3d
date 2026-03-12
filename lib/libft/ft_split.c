/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roazouan <roazouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 17:04:23 by roazouan          #+#    #+#             */
/*   Updated: 2025/12/16 11:43:49 by roazouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	count_char(const char *str, char c, int index)
{
	int	p;

	p = 0;
	while (str[index] && str[index] != c)
	{
		p++;
		index++;
	}
	return (p);
}

int	count_words(char const *s, char c)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] != c && s[i])
		{
			j++;
			while (s[i] && s[i] != c)
				i++;
		}
		else
			i++;
	}
	return (j);
}

static char	*ft_strzdup(const char *str, int index, int n)
{
	int		i;
	char	*s;

	i = 0;
	s = malloc(sizeof(char) * n + 1);
	if (!s)
		return (NULL);
	while (str[index] && i < n)
	{
		s[i] = str[index];
		i++;
		index++;
	}
	s[i] = '\0';
	return (s);
}

char	**ft_split(char const *s, char c)
{
	int		i;
	int		j;
	int		len;
	char	**tab;

	i = 0;
	j = 0;
	len = count_words(s, c);
	tab = malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (!tab)
		return (NULL);
	while (j < len && s[i])
	{
		if (s[i] != c)
		{
			tab[j] = ft_strzdup(s, i, count_char(s, c, i));
			while (s[i] != c && s[i])
				i++;
			j++;
		}
		i++;
	}
	tab[j] = 0;
	return (tab);
}

// int	main(void)
// {
// 	char	**str;
// 	int		i;

// 	// str = ft_split("lorem ipsum dolor sit amet,
// 	// 		consectetur adipiscing elit. Sed non risus. Suspendisse", 'z');
// 	i = 0;
// 	while (str[i])
// 	{
// 		printf("%s\n", str[i]);
// 		i++;
// 	}
// 	return (0);
// }
