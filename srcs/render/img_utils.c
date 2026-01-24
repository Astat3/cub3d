/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   img_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adamgallot <adamgallot@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 16:37:08 by adamgallot        #+#    #+#             */
/*   Updated: 2026/01/24 16:46:03 by adamgallot       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cube3d.h"

void    img_pixel(t_img *img, int x, int y, int color)
{
	int	pixel;
	pixel = (y * img->size_line / 4) + x;
	img->addr[pixel] = color;
}