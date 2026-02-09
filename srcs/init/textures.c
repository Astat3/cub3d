/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roazouan <roazouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 00:00:00 by roazouan          #+#    #+#             */
/*   Updated: 2026/02/07 00:00:00 by roazouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

static void	destroy_texture_img(t_data *data, t_img *img)
{
	if (data->mlx && img->img)
	{
		mlx_destroy_image(data->mlx, img->img);
		img->img = NULL;
		img->addr = NULL;
	}
}

static int	load_single_texture(t_data *data, t_img *img, char *path)
{
	if (!path)
		return (ERRORS);
	img->img = mlx_xpm_file_to_image(data->mlx, path, &img->width, &img->height);
	if (!img->img)
		return (printf("Error: Failed to load texture: %s\n", path), ERRORS);
	img->addr = (int *)mlx_get_data_addr(img->img, &img->pixel_bits,
			&img->size_line, &img->endian);
	if (!img->addr)
	{
		destroy_texture_img(data, img);
		return (printf("Error: Failed to get texture data: %s\n", path), ERRORS);
	}
	return (SUCCESS);
}

static void	free_loaded_textures(t_data *data)
{
	destroy_texture_img(data, &data->textures.north);
	destroy_texture_img(data, &data->textures.south);
	destroy_texture_img(data, &data->textures.east);
	destroy_texture_img(data, &data->textures.west);
}

int	init_textures_img(t_data *data)
{
	if (load_single_texture(data, &data->textures.north, data->no_texture))
		return (ERRORS);
	if (load_single_texture(data, &data->textures.south, data->so_texture))
		return (free_loaded_textures(data), ERRORS);
	if (load_single_texture(data, &data->textures.east, data->ea_texture))
		return (free_loaded_textures(data), ERRORS);
	if (load_single_texture(data, &data->textures.west, data->we_texture))
		return (free_loaded_textures(data), ERRORS);
	data->textures.size = data->textures.north.width;
	return (SUCCESS);
}
