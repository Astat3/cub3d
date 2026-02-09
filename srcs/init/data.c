/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adamgallot <adamgallot@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 19:24:35 by adamgallot        #+#    #+#             */
/*   Updated: 2026/02/07 00:00:00 by roazouan         ###   ########.fr       */
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

static int	parse_color_component(const char *str, int *i)
{
	int	value;
	int	digits;

	value = 0;
	digits = 0;
	while (str[*i] == ' ' || str[*i] == '\t')
		(*i)++;
	while (ft_isdigit(str[*i]))
	{
		value = (value * 10) + (str[*i] - '0');
		if (value > 255)
			return (-1);
		(*i)++;
		digits++;
	}
	if (digits == 0)
		return (-1);
	while (str[*i] == ' ' || str[*i] == '\t')
		(*i)++;
	return (value);
}

static int	parse_rgb_color(const char *str, int *rgb)
{
	int	i;
	int	r;
	int	g;
	int	b;

	if (!str || !rgb)
		return (ERRORS);
	i = 0;
	r = parse_color_component(str, &i);
	if (r < 0 || str[i++] != ',')
		return (ERRORS);
	g = parse_color_component(str, &i);
	if (g < 0 || str[i++] != ',')
		return (ERRORS);
	b = parse_color_component(str, &i);
	if (b < 0)
		return (ERRORS);
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	if (str[i] != '\0')
		return (ERRORS);
	*rgb = (r << 16) | (g << 8) | b;
	return (SUCCESS);
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

int	copy_parsing_to_data(t_data *data, t_parsing *parsing)
{
	if (!data || !parsing)
		return (printf("Error: Parsing or data is NULL.\n"), ERRORS);
	if (!parsing->no_texture || !parsing->so_texture || !parsing->we_texture
		|| !parsing->ea_texture || !parsing->floor_color
		|| !parsing->ceiling_color || !parsing->map)
		return (printf("Error: Missing parsing information.\n"), ERRORS);
	data->no_texture = parsing->no_texture;
	data->so_texture = parsing->so_texture;
	data->we_texture = parsing->we_texture;
	data->ea_texture = parsing->ea_texture;
	data->floor_color = parsing->floor_color;
	data->ceiling_color = parsing->ceiling_color;
	data->map = parsing->map;
	if (parse_rgb_color(data->floor_color, &data->floor_rgb) != SUCCESS)
		return (printf("Error: Invalid floor color.\n"), ERRORS);
	if (parse_rgb_color(data->ceiling_color, &data->ceiling_rgb) != SUCCESS)
		return (printf("Error: Invalid ceiling color.\n"), ERRORS);
	return (SUCCESS);
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
	data->frame.img = mlx_new_image(data->mlx, data->win_width, data->win_height);
	if (!data->frame.img)
		return (printf("Error: mlx_new_image failed.\n"), ERRORS);
	data->frame.addr = (int *)mlx_get_data_addr(data->frame.img,
			&data->frame.pixel_bits, &data->frame.size_line, &data->frame.endian);
	if (!data->frame.addr)
		return (printf("Error: mlx_get_data_addr failed.\n"), ERRORS);
	data->frame.width = data->win_width;
	data->frame.height = data->win_height;
	return (SUCCESS);
}
