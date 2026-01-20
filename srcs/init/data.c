/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adamgallot <adamgallot@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 19:24:35 by adamgallot        #+#    #+#             */
/*   Updated: 2026/01/20 12:43:58 by adamgallot       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"


/* Le parsing doit ecraser les valeurs de dir et de plane*
cette fonction doit être appelée avant de lire la map;
*/


static void init_player(t_player *player)
{
	player->get_dir = '\0';
	player->pos_x = 0.0;
	player->pos_y = 0.0;
	player->dir_x = 0.0;
	player->dir_y = 0.0;
	player->plane_x = 0.0;
	player->plane_y = 0.0;
	player->has_moved = false;
	player->move_x = 0;
	player->move_y = 0;
	player->rotation = 0;
}

void	create_img(t_img *img)
{
	img->img = NULL;
	img->addr = NULL;
	img->pixel_bits = 0;
	img->size_line = 0;
	img->endian = 0;
}

void	init_raycast(t_ray *ray)
{
	ray->map_x = 0;
	ray->map_y = 0;
	ray->camera_x = 0.0;
	ray->raydir_x = 0.0;
	ray->raydir_y = 0.0;
	ray->step_x = 0;
	ray->step_y = 0;
	ray->sidedist_x = 0.0;
	ray->sidedist_y = 0.0;
	ray->deltadist_x = 0.0;
	ray->deltadist_y = 0.0;
	ray->side = 0;
	ray->perpWallDist = 0.0;
}

void init_data(t_data *data)
{
	data->mlx = NULL;
	data->win = NULL;
	data->win_width = WIDTH;
	data->win_height = HEIGHT;
	//text info init / mapinfo
	init_player(&data->player);
	// init autres trucs de data
}