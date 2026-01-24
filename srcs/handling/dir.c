/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dir.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adamgallot <adamgallot@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 17:10:44 by adamgallot        #+#    #+#             */
/*   Updated: 2026/01/24 17:15:16 by adamgallot       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

static void init_dir_player_NOSO(t_player *player)
{
	if (player->get_dir == 'N')
    {
        player->dir_x = -1;
        player->dir_y = 0;
        player->plane_x = 0;
        player->plane_y = 0.66;
    }
    else if (player->get_dir == 'S')
    {
        player->dir_x = 1;
        player->dir_y = 0;
        player->plane_x = 0;
        player->plane_y = -0.66;
    }
    else
        return;
}

static void init_dir_player_EAWE(t_player *player)
{
    if (player->get_dir == 'E')
    {
        player->dir_x = 0;
        player->dir_y = 1;
        player->plane_x = 0.66;
        player->plane_y = 0;
    }
    else if (player->get_dir == 'W')
    {
        player->dir_x = 0;
        player->dir_y = -1;
        player->plane_x = -0.66;
        player->plane_y = 0;
    }
    else
        return;
}

void init_dir_player(t_player *player)
{
    init_dir_player_NOSO(player);
    init_dir_player_EAWE(player);
}