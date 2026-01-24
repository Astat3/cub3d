/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotation.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adamgallot <adamgallot@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 17:55:08 by adamgallot        #+#    #+#             */
/*   Updated: 2026/01/24 18:01:17 by adamgallot       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

static int rotation(t_data *data, double speed)
{
	t_player	*player;
	double		tmp_x;

	player = &data->player;
	tmp_x = player->dir_x;
	player->dir_x = player->dir_x * cos(speed) - player->dir_y * sin(speed);
	player->dir_y = tmp_x * sin(speed) + player->dir_y * cos(speed);
	tmp_x = player->plane_x;
	player->plane_x = player->plane_x * cos(speed) - player->plane_y * sin(speed);
	player->plane_y = tmp_x * sin(speed) + player->plane_y * cos(speed);
	return (1);
}
int	og_rotation(t_data *data, double rotation_dir)
{
	int rotated;
	double speed;

	rotated = 0;
	speed = rot_speed * rotation_dir;
	rotated += rotation(data, speed);
	return (rotated);
}