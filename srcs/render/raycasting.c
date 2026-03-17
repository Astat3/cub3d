/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roazouan <roazouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 00:00:00 by roazouan          #+#    #+#             */
/*   Updated: 2026/03/17 00:00:00 by roazouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

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

static int	get_tex_color(t_img *tex, t_ray *ray, int y, int win_height)
{
	int	d;
	int	tex_y;

	d = y * 256 - win_height * 128 + ray->line_height * 128;
	tex_y = ((d * tex->height) / ray->line_height) / 256;
	if (tex_y < 0)
		tex_y = 0;
	if (tex_y >= tex->height)
		tex_y = tex->height - 1;
	return (tex->addr[tex_y * (tex->size_line / 4) + ray->tex_x]);
}

void	render_column(t_data *data, int x)
{
	int		y;
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
		color = get_tex_color(tex, &ray, y, data->win_height);
		my_mlx_pixel_put(&data->frame, x, y, color);
		y++;
	}
}
