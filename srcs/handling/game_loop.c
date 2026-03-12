/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roazouan <roazouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 00:00:00 by roazouan          #+#    #+#             */
/*   Updated: 2026/02/07 00:00:00 by roazouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

static int	is_walkable(t_data *data, double x, double y)
{
	int	map_x;
	int	map_y;

	if (!data || !data->map || x < 0 || y < 0)
		return (0);
	map_x = (int)x;
	map_y = (int)y;
	if (!data->map[map_y] || map_x >= (int)ft_strlen(data->map[map_y]))
		return (0);
	return (data->map[map_y][map_x] == '0');
}

static void	move_player(t_data *data)
{
	double	step_x;
	double	step_y;
	double	offset_x;
	double	offset_y;

	step_x = (data->player.dir_x * data->player.move_y
			+ data->player.plane_x * data->player.move_x) * MOVE_SPEED;
	step_y = (data->player.dir_y * data->player.move_y
			+ data->player.plane_y * data->player.move_x) * MOVE_SPEED;
	offset_x = 0.20;
	offset_y = 0.20;
	if (step_x < 0)
		offset_x = -0.20;
	if (step_y < 0)
		offset_y = -0.20;
	if (is_walkable(data, data->player.pos_x + step_x + offset_x, data->player.pos_y))
		data->player.pos_x += step_x;
	if (is_walkable(data, data->player.pos_x, data->player.pos_y + step_y + offset_y))
		data->player.pos_y += step_y;
}

static void	rotate_player(t_data *data)
{
	double	rot;
	double	old_dir_x;
	double	old_plane_x;

	if (data->player.rotation == 0)
		return ;
	rot = data->player.rotation * ROT_SPEED;
	old_dir_x = data->player.dir_x;
	data->player.dir_x = data->player.dir_x * cos(rot)
		- data->player.dir_y * sin(rot);
	data->player.dir_y = old_dir_x * sin(rot) + data->player.dir_y * cos(rot);
	old_plane_x = data->player.plane_x;
	data->player.plane_x = data->player.plane_x * cos(rot)
		- data->player.plane_y * sin(rot);
	data->player.plane_y = old_plane_x * sin(rot)
		+ data->player.plane_y * cos(rot);
}

void	update_player(t_data *data)
{
	data->player.has_moved = false;
	if (data->player.move_x != 0 || data->player.move_y != 0)
	{
		move_player(data);
		data->player.has_moved = true;
	}
	if (data->player.rotation != 0)
	{
		rotate_player(data);
		data->player.has_moved = true;
	}
}

int	game_loop(t_data *data)
{
	update_player(data);
	render_frame(data);
	return (0);
}
