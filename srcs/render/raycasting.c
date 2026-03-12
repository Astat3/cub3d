/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roazouan <roazouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 00:00:00 by roazouan          #+#    #+#             */
/*   Updated: 2026/02/07 00:00:00 by roazouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

static char	get_map_cell(t_data *data, int map_x, int map_y)
{
	if (!data->map || map_x < 0 || map_y < 0)
		return ('1');
	if (!data->map[map_y])
		return ('1');
	if (map_x >= (int)ft_strlen(data->map[map_y]))
		return ('1');
	return (data->map[map_y][map_x]);
}

static void	init_ray(t_data *data, t_ray *ray, int x)
{
	ray->camera_x = (2.0 * x) / (double)data->win_width - 1.0;
	ray->dir_x = data->player.dir_x + data->player.plane_x * ray->camera_x;
	ray->dir_y = data->player.dir_y + data->player.plane_y * ray->camera_x;
	ray->map_x = (int)data->player.pos_x;
	ray->map_y = (int)data->player.pos_y;
	if (ray->dir_x == 0.0)
		ray->delta_dist_x = 1e30;
	else
		ray->delta_dist_x = fabs(1.0 / ray->dir_x);
	if (ray->dir_y == 0.0)
		ray->delta_dist_y = 1e30;
	else
		ray->delta_dist_y = fabs(1.0 / ray->dir_y);
	ray->hit = 0;
}

static void	init_step_and_side_dist(t_data *data, t_ray *ray)
{
	if (ray->dir_x < 0)
	{
		ray->step_x = -1;
		ray->side_dist_x = (data->player.pos_x - ray->map_x) * ray->delta_dist_x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist_x = (ray->map_x + 1.0 - data->player.pos_x)
			* ray->delta_dist_x;
	}
	if (ray->dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_dist_y = (data->player.pos_y - ray->map_y) * ray->delta_dist_y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist_y = (ray->map_y + 1.0 - data->player.pos_y)
			* ray->delta_dist_y;
	}
}

static void	perform_dda(t_data *data, t_ray *ray)
{
	char	cell;

	while (!ray->hit)
	{
		if (ray->side_dist_x < ray->side_dist_y)
		{
			ray->side_dist_x += ray->delta_dist_x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist_y += ray->delta_dist_y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		cell = get_map_cell(data, ray->map_x, ray->map_y);
		if (cell != '0')
			ray->hit = 1;
	}
}

static t_img	*select_texture(t_data *data, t_ray *ray)
{
	if (ray->side == 0 && ray->dir_x > 0)
		return (&data->textures.west);
	if (ray->side == 0 && ray->dir_x < 0)
		return (&data->textures.east);
	if (ray->side == 1 && ray->dir_y > 0)
		return (&data->textures.north);
	return (&data->textures.south);
}

static void	calculate_line_and_texture(t_data *data, t_ray *ray, t_img *tex)
{
	if (ray->side == 0)
		ray->perp_wall_dist = (ray->map_x - data->player.pos_x
				+ (1 - ray->step_x) / 2.0) / ray->dir_x;
	else
		ray->perp_wall_dist = (ray->map_y - data->player.pos_y
				+ (1 - ray->step_y) / 2.0) / ray->dir_y;
	if (ray->perp_wall_dist <= 0.0)
		ray->perp_wall_dist = 0.01;
	ray->line_height = (int)(data->win_height / ray->perp_wall_dist);
	ray->draw_start = -ray->line_height / 2 + data->win_height / 2;
	if (ray->draw_start < 0)
		ray->draw_start = 0;
	ray->draw_end = ray->line_height / 2 + data->win_height / 2;
	if (ray->draw_end >= data->win_height)
		ray->draw_end = data->win_height - 1;
	if (ray->side == 0)
		ray->wall_x = data->player.pos_y + ray->perp_wall_dist * ray->dir_y;
	else
		ray->wall_x = data->player.pos_x + ray->perp_wall_dist * ray->dir_x;
	ray->wall_x -= floor(ray->wall_x);
	ray->tex_x = (int)(ray->wall_x * (double)tex->width);
	if (ray->side == 0 && ray->dir_x > 0)
		ray->tex_x = tex->width - ray->tex_x - 1;
	if (ray->side == 1 && ray->dir_y < 0)
		ray->tex_x = tex->width - ray->tex_x - 1;
}

void	render_column(t_data *data, int x)
{
	int		y;
	int		d;
	t_ray	ray;
	t_img	*tex;
	int		color;

	init_ray(data, &ray, x);
	init_step_and_side_dist(data, &ray);
	perform_dda(data, &ray);
	tex = select_texture(data, &ray);
	if (!tex->addr)
		return ;
	calculate_line_and_texture(data, &ray, tex);
	y = ray.draw_start;
	while (y <= ray.draw_end)
	{
		d = y * 256 - data->win_height * 128 + ray.line_height * 128;
		ray.tex_y = ((d * tex->height) / ray.line_height) / 256;
		if (ray.tex_y < 0)
			ray.tex_y = 0;
		if (ray.tex_y >= tex->height)
			ray.tex_y = tex->height - 1;
		color = tex->addr[ray.tex_y * (tex->size_line / 4) + ray.tex_x];
		my_mlx_pixel_put(&data->frame, x, y, color);
		y++;
	}
}
