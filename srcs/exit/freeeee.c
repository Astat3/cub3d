/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeeee.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roazouan <roazouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 19:06:07 by adamgallot        #+#    #+#             */
/*   Updated: 2026/02/07 00:00:00 by roazouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

void	free_tab(char **tab)
{
	int	i;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

static void	free_textures_img(t_data *data)
{
	if (!data->mlx)
		return ;
	if (data->frame.img)
		mlx_destroy_image(data->mlx, data->frame.img);
	if (data->textures.north.img)
		mlx_destroy_image(data->mlx, data->textures.north.img);
	if (data->textures.south.img)
		mlx_destroy_image(data->mlx, data->textures.south.img);
	if (data->textures.east.img)
		mlx_destroy_image(data->mlx, data->textures.east.img);
	if (data->textures.west.img)
		mlx_destroy_image(data->mlx, data->textures.west.img);
	data->frame.img = NULL;
	data->textures.north.img = NULL;
	data->textures.south.img = NULL;
	data->textures.east.img = NULL;
	data->textures.west.img = NULL;
}

static void	free_strings(t_data *data)
{
	if (data->no_texture)
		free(data->no_texture);
	if (data->so_texture)
		free(data->so_texture);
	if (data->we_texture)
		free(data->we_texture);
	if (data->ea_texture)
		free(data->ea_texture);
	if (data->floor_color)
		free(data->floor_color);
	if (data->ceiling_color)
		free(data->ceiling_color);
	data->no_texture = NULL;
	data->so_texture = NULL;
	data->we_texture = NULL;
	data->ea_texture = NULL;
	data->floor_color = NULL;
	data->ceiling_color = NULL;
}

void	free_data(t_data *data)
{
	if (!data)
		return ;
	free_textures_img(data);
	free_strings(data);
	free_tab(data->map);
	data->map = NULL;
}
