/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index_player.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adamgallot <adamgallot@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 17:16:27 by adamgallot        #+#    #+#             */
/*   Updated: 2026/01/24 17:27:48 by adamgallot       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"


// 0.25 is the hitbox size
static bool is_valid_in_map(t_data *data, double x, double y)
{
	if (x < 0.25 || x >= data->map_info.width - 1.25)
		return (false);
	if (y < 0.25 || y >= data->map_info.height - 0.25)
		return (false);
	return (true);
}

static bool is_in_map(t_data *data, double x, double y)
{
	if (data->map[(int)(y)][(int)(x)] == '0')
		return (true);
	return (false);
}

int	valide_move(t_data *data, double acutal_x, double actual_y)
{
	int flag;

	flag = 0;
	if(is_valid_in_map(data, acutal_x, data->player.pos_y))
	{
		data->player.pos_x = acutal_x;
		flag++;
	}
	if (is_valid_in_map(data, data->player.pos_x, actual_y))
	{
		data->player.pos_y = actual_y;
		flag++;
	}
	return (flag);
}