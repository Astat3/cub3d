/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adamgallot <adamgallot@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 18:16:53 by adamgallot        #+#    #+#             */
/*   Updated: 2026/01/19 18:46:39 by adamgallot       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"
#include "../../includes/handler.h"

static int	key_press_handling(int key_input, t_data *data)
{
	if (key_input == XK_Escape)
		quit_code(data);
	else if (key_input == XK_w)
		data->player.move_y = 1;
	else if (key_input == XK_s)
		data->player.move_y = -1;
	else if (key_input == XK_a)
		data->player.move_x = -1;
	else if (key_input == XK_d)
		data->player.move_x = 1;
	else if (key_input == XK_Left)
		data->player.rotation = -1;
	else if (key_input == XK_Right)
		data->player.rotation = 1;
	return (0);
}

static int	key_release_handler(int key_input, t_data *data)
{
	if (key_input == XK_Escape)
		quit_code(data);
	if (key_input == XK_w && data->player.move_y == 1)
		data->player.move_y = 0;
	else if (key_input == XK_s && data->player.move_y == -1)
		data->player.move_y = 0;
	else if (key_input == XK_a && data->player.move_x == -1)
		data->player.move_x = 0;
	else if (key_input == XK_d && data->player.move_x == 1)
		data->player.move_x = 0;
	else if (key_input == XK_Left && data->player.rotation == -1)
		data->player.rotation = 0;
	else if (key_input == XK_Right && data->player.rotation == 1)
		data->player.rotation = 0;
	return (0);
}

void	listen_input(t_data *data)
{
	mlx_hook(data->win, 2, 1L<<0, &key_press_handling, data);
	mlx_hook(data->win, 3, 1L<<1, &key_release_handler, data);
	mlx_hook(data->win, 17, 0, &quit_code, data);
	mlx_loop_hook(data->mlx, &game_loop, data);
}
