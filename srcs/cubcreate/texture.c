/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adamgallot <adamgallot@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 19:07:55 by adamgallot        #+#    #+#             */
/*   Updated: 2026/01/21 20:14:09 by adamgallot       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

/*------------TEXTURE HANDLING----------------------*/
/*
---> 1ere étape 
	-> déterminer quelle colonne a regarder en fonction du rayon touchant le mur
---> 2eme étape
	-> étirer ou rétrecire la texture 
--> 3eme étape
	-> dessiner la texture sur l'écran

GESTION DE LA MÉMOIRE
	index -> tableau 2d 
	index = (y * largeur_image) + x
Parsing doit convertir la texture en int *

calcul coordonée horizontal -> tex_x = wall_x * texture_width

pour tex_y on utilise step = 1.0 * texture_height / line_height | pour le scaling
	

*/


/* init texture tab =>  memory allocation for the buffer of the screen */
void	init_texture_tab(t_data *data)
{
	int i;

	i = 0;
	if (data->texture_tab)
		free_tab((void **)data->texture_tab);
	data->texture_tab = ft_calloc(data->win_height + 1, sizeof * data->texture_tab);
	if (!data->texture_tab)
			before_quit(data, 2);
	while ( i < data->win_height)
	{
		data->texture_tab[i] = ft_calloc(data->win_width, sizeof * data->texture_tab);
		if (!data->texture_tab[i])
			before_quit(data, 2);
		i++;
	}
}

/*
QUELLE IMAGE DOIT ÊTRE AFFICHÉ EN FONCTION DU MUR TOUCHÉ PAR LE RAYON
*/
static void get_index_texture(t_data *data, t_ray *ray)
{
	if (ray->side == 0)
	{
		if(ray->raydir_x < 0)
			data->texture_info.texture_index = WEST;
		else
			data->texture_info.texture_index = EAST;
	}
	if (ray->side == 1)
	{
		if(ray->raydir_y < 0)
			data->texture_info.texture_index = NORTH;
		else
			data->texture_info.texture_index = SOUTH;
	}
}

/* 
Refresh the new texture for the next ray
wall_x -> position exacte du mur touché
texture_size = taille de la texture en pixels
*/
void	new_texture(t_data *data, t_ray *ray, t_texture_info *texture, int x)
{
	int 	y; 
	int   color;
	get_index_texture(data, ray);
	texture->x_tex = (int) (ray->wall_x * texture->texture_size);
	if ((ray->side == 0 && ray->raydir_x > 0) || (ray->side == 1 && ray->raydir_y < 0))
		texture->x_tex = texture->texture_size - texture->x_tex - 1; // correction effet mirroir
	texture->step = 1.0 * texture->texture_size / ray->line_height;
	texture->tex_pos = (ray->start_draw - data->win_height / 2 + ray->line_height / 2) * texture->step;
	y = ray->start_draw;
	while (y < ray->end_draw)
	{
		texture->y_tex = (int)texture->tex_pos & (texture->texture_size - 1);
		texture->tex_pos += texture->step;
		color = data->texture[data->texture_info.texture_index][texture->texture_size * texture->y_tex + texture->x_tex];
		if (data->texture_info.texture_index == EAST || data->texture_info.texture_index == WEST)
			color = color >> 1 & 8355711; // divise par 2 un rouge vif pour l'assombrir et évite le vert parasite dans le bit shifting
		if (color > 0)
			data->texture_tab[y][x] = color;
		y++;
	}
}
