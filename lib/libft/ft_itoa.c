/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roazouan <roazouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 16:50:55 by roazouan          #+#    #+#             */
/*   Updated: 2025/11/09 18:58:46 by roazouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	get_len(long long int nbr)
{
	int	len;

	len = 0;
	if (nbr == 0)
		return (1);
	if (nbr < 0)
	{
		nbr *= -1;
		len = 1;
	}
	while (nbr > 0)
	{
		nbr /= 10;
		len++;
	}
	return (len);
}

char	*ft_revtabc(char *str)
{
	int		i;
	int		len;
	char	tmp;

	i = 0;
	len = (ft_strlen(str) - 1);
	if (str[i] == '-')
		i++;
	while (i <= len)
	{
		tmp = str[len];
		str[len--] = str[i];
		str[i++] = tmp;
	}
	return (str);
}

char	*ft_itoa(int n)
{
	long long int	nbr;
	char			*res;
	int				i;
	int				len;

	nbr = n;
	i = 0;
	len = get_len(nbr);
	res = malloc(sizeof(char) * (len + 1));
	if (!res)
		return (NULL);
	if (nbr == 0)
		res[i++] = '0';
	if (nbr < 0)
	{
		res[i++] = '-';
		nbr *= -1;
	}
	while (nbr > 0)
	{
		res[i++] = nbr % 10 + '0';
		nbr /= 10;
	}
	res[i] = '\0';
	return (ft_revtabc(res));
}

// int main()
// {
// 	char *u = ft_itoa(-2147483648);
//     printf("%s\n", u);
// 	free(u);
//     return (0);
// }