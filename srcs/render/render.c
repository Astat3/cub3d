/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roazouan <roazouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 00:00:00 by roazouan          #+#    #+#             */
/*   Updated: 2026/02/07 00:00:00 by roazouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

void	my_mlx_pixel_put(t_img *img, int x, int y, int color)
{
	int	index;

	if (!img || !img->addr)
		return ;
	if (x < 0 || y < 0 || x >= img->width || y >= img->height)
		return ;
	index = y * (img->size_line / 4) + x;
	img->addr[index] = color;
}

static void	draw_floor_and_ceiling(t_data *data)
{
	int	x;
	int	y;

	y = 0;
	while (y < data->win_height)
	{
		x = 0;
		while (x < data->win_width)
		{
			if (y < data->win_height / 2)
				my_mlx_pixel_put(&data->frame, x, y, data->ceiling_rgb);
			else
				my_mlx_pixel_put(&data->frame, x, y, data->floor_rgb);
			x++;
		}
		y++;
	}
}

void	render_frame(t_data *data)
{
	int	x;

	if (!data || !data->frame.img)
		return ;
	draw_floor_and_ceiling(data);
	x = 0;
	while (x < data->win_width)
	{
		render_column(data, x);
		x++;
	}
	mlx_put_image_to_window(data->mlx, data->win, data->frame.img, 0, 0);
}
