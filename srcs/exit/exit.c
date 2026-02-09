/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roazouan <roazouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 18:48:25 by adamgallot        #+#    #+#             */
/*   Updated: 2026/02/07 00:00:00 by roazouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

int	quit_code(t_data *data)
{
	if (!data)
		exit(0);
	free_data(data);
	if (data->win && data->mlx)
		mlx_destroy_window(data->mlx, data->win);
	data->win = NULL;
	exit(0);
	return (0);
}
