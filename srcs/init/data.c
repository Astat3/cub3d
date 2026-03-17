/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adamgallot <adamgallot@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 19:24:35 by adamgallot        #+#    #+#             */
/*   Updated: 2026/03/17 00:00:00 by roazouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

static void	init_player(t_player *player)
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

static void	init_img(t_img *img)
{
	img->img = NULL;
	img->addr = NULL;
	img->pixel_bits = 0;
	img->size_line = 0;
	img->endian = 0;
	img->width = 0;
	img->height = 0;
}

void	init_data(t_data *data)
{
	data->mlx = NULL;
	data->win = NULL;
	data->win_width = WIDTH;
	data->win_height = HEIGHT;
	data->no_texture = NULL;
	data->so_texture = NULL;
	data->we_texture = NULL;
	data->ea_texture = NULL;
	data->floor_color = NULL;
	data->ceiling_color = NULL;
	data->floor_rgb = 0;
	data->ceiling_rgb = 0;
	data->map = NULL;
	init_img(&data->textures.north);
	init_img(&data->textures.south);
	init_img(&data->textures.east);
	init_img(&data->textures.west);
	data->textures.size = 0;
	init_img(&data->frame);
	init_player(&data->player);
}

int	init_mlx(t_data *data)
{
	if (!data)
		return (printf("Error: Data is NULL.\n"), ERRORS);
	data->mlx = mlx_init();
	if (!data->mlx)
		return (printf("Error: mlx_init failed.\n"), ERRORS);
	data->win = mlx_new_window(data->mlx, data->win_width, data->win_height,
			"cub3D");
	if (!data->win)
		return (printf("Error: mlx_new_window failed.\n"), ERRORS);
	return (SUCCESS);
}

int	init_frame_img(t_data *data)
{
	if (!data || !data->mlx)
		return (ERRORS);
	data->frame.img = mlx_new_image(data->mlx, data->win_width,
			data->win_height);
	if (!data->frame.img)
		return (printf("Error: mlx_new_image failed.\n"), ERRORS);
	data->frame.addr = (int *)mlx_get_data_addr(data->frame.img,
			&data->frame.pixel_bits, &data->frame.size_line,
			&data->frame.endian);
	if (!data->frame.addr)
		return (printf("Error: mlx_get_data_addr failed.\n"), ERRORS);
	data->frame.width = data->win_width;
	data->frame.height = data->win_height;
	return (SUCCESS);
}
