/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adamgallot <adamgallot@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 16:42:24 by adamgallot        #+#    #+#             */
/*   Updated: 2026/01/24 18:02:48 by adamgallot       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cube3d.h"

static void set_frame_image_pixel(t_data *data, t_img *img, int x, int y)
{
	if (data->texture_tab[y][x] > 0)
		img_pixel(img, x, y, data->texture_tab[y][x]);
	else if (y < data->win_height / 2)
		img_pixel(img, x, y, data->texture_info.ceiling_hex);
	else if (y < data->win_height - 1)
		img_pixel(img, x, y, data->texture_info.floor_hex);
}

static void render_frame(t_data *data)
{
    t_img	img;
    int		x;
    int		y;

    img.img = NULL;
    init_img(data, &img, data->win_width, data->win_height);
    y = 0;
    while (y < data->win_height)
    {
        x = 0;
        while (x < data->win_width)
        {
            set_frame_image_pixel(data, &img, x, y);
            x++;
        }
        y++;
    }
    mlx_put_image_to_window(data->mlx, data->win, img.img, 0, 0);
    mlx_destroy_image(data->mlx, img.img);
}

static void ray_render(t_data *data)
{
    init_texture_tab(data);
    init_raycast(&data->ray);
    actual_raycasting(&data->player, data);
    render_frame(data);
}

void render(t_data *data)
{
    ray_render(data);
}

int og_render(t_data *data)
{
    data->player.has_moved += og_move(data);
    if (data->player.has_moved == 0)
        return (0);
    render(data);
    return (0);
}