/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adamgallot <adamgallot@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 17:15:28 by adamgallot        #+#    #+#             */
/*   Updated: 2026/01/24 17:58:58 by adamgallot       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

static int move_forward(t_data *data)
{
	double actual_x;
	double actual_y;

	actual_x = data->player.pos_x + data->player.dir_x * move_speed;
	actual_y = data->player.pos_y + data->player.dir_y * move_speed;
	return (valide_move(data, actual_x, actual_y));
}

static int move_backward(t_data *data)
{
	double actual_x;
	double actual_y;

	actual_x = data->player.pos_x - data->player.dir_x * move_speed;
	actual_y = data->player.pos_y - data->player.dir_y * move_speed;
	return (valide_move(data, actual_x, actual_y));
}

static int move_left(t_data *data)
{
	double actual_x;
	double actual_y;

	actual_x = data->player.pos_x + data->player.dir_y * move_speed;
	actual_y = data->player.pos_y - data->player.dir_x * move_speed;
	return (valide_move(data, actual_x, actual_y));
}

static int move_right(t_data *data)
{
	double actual_x;
	double actual_y;

	actual_x = data->player.pos_x - data->player.dir_y * move_speed;
	actual_y = data->player.pos_y + data->player.dir_x * move_speed;
	return (valide_move(data, actual_x, actual_y));
}
int og_move(t_data *data)
{
	int moved;

	moved = 0;
	if (data->player.move_y == 1)
		moved += move_forward(data);
	else if (data->player.move_y == -1)
		moved += move_backward(data);
	if (data->player.move_x == -1)
		moved += move_left(data);
	else if (data->player.move_x == 1)
		moved += move_right(data);
	return (moved);
}