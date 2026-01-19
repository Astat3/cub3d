/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adamgallot <adamgallot@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 18:48:25 by adamgallot        #+#    #+#             */
/*   Updated: 2026/01/19 18:56:57 by adamgallot       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

void	before_quit(t_data *data, int flag)
{
	if (!data)
		exit(flag);
	if (data->win && data->mlx) // Si la fenêtre et mlx existent
		mlx_destroy_window(data->mlx, data->win);
	if (data->mlx) // Si mlx existe -> mauvais parsing par exemple
	{
		mlx_destroy_display(data->mlx);
		mlx_loop_end(data->mlx);
		free(data->mlx);
	}
	free_all(data);
}

int	quit_code(t_data *data)
{
	before_quit(data, 0);
	exit(0);
}
