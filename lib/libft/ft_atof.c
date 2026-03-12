/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roazouan <roazouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 12:18:22 by roazouan          #+#    #+#             */
/*   Updated: 2025/12/07 12:19:56 by roazouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static double	parse_decimal(const char *str, double result)
{
	double	decimal_place;

	decimal_place = 0.1;
	while (*str >= '0' && *str <= '9')
	{
		result += (*str - '0') * decimal_place;
		decimal_place *= 0.1;
		str++;
	}
	return (result);
}

double	ft_atof(const char *str)
{
	double	result;
	double	sign;

	result = 0.0;
	sign = 1.0;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1.0;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10.0 + (*str - '0');
		str++;
	}
	if (*str == '.')
		result = parse_decimal(str + 1, result);
	return (result * sign);
}
