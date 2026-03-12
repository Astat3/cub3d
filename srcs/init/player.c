/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roazouan <roazouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 00:00:00 by roazouan          #+#    #+#             */
/*   Updated: 2026/02/07 00:00:00 by roazouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

static void	set_dir_north(t_player *player)
{
	player->dir_x = 0.0;
	player->dir_y = -1.0;
	player->plane_x = 0.66;
	player->plane_y = 0.0;
}

static void	set_dir_south(t_player *player)
{
	player->dir_x = 0.0;
	player->dir_y = 1.0;
	player->plane_x = -0.66;
	player->plane_y = 0.0;
}

static void	set_dir_east(t_player *player)
{
	player->dir_x = 1.0;
	player->dir_y = 0.0;
	player->plane_x = 0.0;
	player->plane_y = 0.66;
}

static void	set_dir_west(t_player *player)
{
	player->dir_x = -1.0;
	player->dir_y = 0.0;
	player->plane_x = 0.0;
	player->plane_y = -0.66;
}

void	init_player_dir(t_data *data, t_parsing *parsing)
{
	char	dir;

	data->player.pos_x = (double)parsing->player_start_x + 0.5;
	data->player.pos_y = (double)parsing->player_start_y + 0.5;
	dir = parsing->map[parsing->player_start_y][parsing->player_start_x];
	data->player.get_dir = dir;
	parsing->map[parsing->player_start_y][parsing->player_start_x] = '0';
	if (dir == 'N')
		set_dir_north(&data->player);
	else if (dir == 'S')
		set_dir_south(&data->player);
	else if (dir == 'E')
		set_dir_east(&data->player);
	else if (dir == 'W')
		set_dir_west(&data->player);
}
