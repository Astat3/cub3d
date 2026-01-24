/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mlx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adamgallot <adamgallot@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 16:54:47 by adamgallot        #+#    #+#             */
/*   Updated: 2026/01/24 17:02:47 by adamgallot       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

void	init_img(t_data *data, t_img *image, int width, int height)
{
	create_img(image);
	image->img = mlx_new_image(data->mlx, width, height);
	if (image->img == NULL)
		quit_code(data);
	image->addr = (int *)mlx_get_data_addr(image->img, &image->pixel_bits,
			&image->size_line, &image->endian);
	return ;
}

void	init_txture_img(t_data *data,t_img *image, char *path)
{
	create_img(image);
	image->img = mlx_xpm_file_to_image(data->mlx, path,
			&data->texture_info.texture_size,
				&data->texture_info.texture_size);
	if (image->img == NULL)
		quit_code(data);
	image->addr = (int *)mlx_get_data_addr(image->img, &image->pixel_bits,
			&image->size_line, &image->endian);
	return ;
}
void	init_mlx(t_data *data)
{
	data->mlx = mlx_init();
	if (!data->mlx)
		quit_code(data);
	data->win = mlx_new_window(data->mlx, WIDTH, HEIGHT, "Cub3D");
	if (!data->win)
		quit_code(data);
	return;
}