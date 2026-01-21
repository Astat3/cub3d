/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adamgallot <adamgallot@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 10:26:23 by adamgallot        #+#    #+#             */
/*   Updated: 2026/01/21 20:21:15 by adamgallot       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

/*

			Variable de distance 
		-> deltadist  == distance que le rayon doit parcourir pour passer d'une ligne à l'autre
		--> sidedist == distance entre la position actuelle et le premier côté x ou y

			Détérmination de la direction
		->il faut savoir la direction du rayon pour saovir quelle case on doit verifier	
		--> stepx et stepy
				en fonction du signe de raydirx et raydiry -> vecteur qui définit la direction du rayon lancé pour une colonne
				raydirx > 0 => stepx = 1 | vers la droite
				(resp < 0)

			camera_X 
		--> normalisation qui permet de balayer le fov de gauche a droite
			double camera_x = 2 * x / (double)w - 1
			si x = O -> cam_x = -1 -> donc on bien a gauche
			si x = w / 2 -> cam_w = 0 -> donc bien centre de l'ecran 
			si x = w => cam_X = 1 -> droite de l'ecran
		Lancer la boucle
		--> while tant qu'on touche pas un mur
			- on compare sidedistx et sidedisty
			- on ajoute le deltadist correspondant
			- on met a jour la case courante
				map_x et map_y
			- on definit si on a touché un mur en fonction de la map
		
		
			

Le Schéma des Vecteurs (Vue de dessus)
	  (Bord Gauche)                    (Écran / Camera Plane)                  (Bord Droit)
		   |                                     |                                     |
		   |<------------------------------------|------------------------------------>|
		   |          - plane                    |              + plane                |
		   |                                     |                                     |
	   Point A                               Point B                                Point C
		   \                                     ^                                     /
			\                                    |                                    /
			 \                                   |                                   /
			  \                                  | dir                              /
			   \                                 |                                 /
				\                                |                                /
	  rayDir     \                               |                               /
	(pour x=0)    \                              |                              /
				   \                             |                             /
					\                            |                            /
					 \                           |                           /
					  \                          |                          /
					   \                         |                         /
						\                        |                        /
						 \                       |                       /
						  \                      |                      /
						   \                     |                     /
							\                    |                    /
							 \                   |                   /
							  \                  |                  /
							   \                 |                 /
								\                |                /
								 \               |               /
								  \              |              /
								   \             |             /
									\            |            /
									 \           |           /
									  \          |          /
									   \         |         /
										\        O
									  Position du Joueur
										  (pos)

*/

/*---------------------------------------------------------------------------------------------------*/

/*
	Initialisation des variables de raycast
Pour chaque x de l'ecran, --> camera_x => donne une mise à l'echelle
						-> dix_x / dir_y -> angle de porté du rayon
						map_x  / y --> depart dans la grille
						calcul des delta pour le DDA
						
*/

static void initialization_raycast(int x, t_ray *ray, t_player *player)
{
	init_raycast(ray);
	ray->camera_x = 2 * x / (double)(WIDTH) - 1;
	ray->raydir_x = player->dir_x + player->plane_x * ray->camera_x;
	ray->raydir_y = player->dir_y + player->plane_y * ray->camera_x;
	ray->map_x = (int)player->pos_x;
	ray->map_y = (int)player->pos_y;
	ray->deltadist_x = fabs(1 / ray->raydir_x);
	ray->deltadist_y = fabs(1 / ray->raydir_y);
}

/*
	Determination de la direction du rayon
Le perso est à 3.2 sur axe de X	
	< 0 --> regarde la gauche
	donc procaine ligne a 3 -> il faut parcourir 0.2
	> 0 --> regarde la droite
	donc prochaine ligne a 4 -> il faut parcourir 0.8
	Rôle de deltalist
		conversion en distance composé de x et y
		consant, permet de savoir combien de distance on doit parcourir pour passer d'une ligne à l'autre en x ou y
	sidedist 
		accumulation,
		position de départ jusqu'au prochain coté de la case.
		
*/

static void setting_dda(t_ray *ray, t_player *player)
{
	if (ray->raydir_x < 0)
	{
		ray->step_x = -1;
		ray->sidedist_x = (player->pos_x - ray->map_x) * ray->deltadist_x;
	}
	else
	{
		ray->step_x = 1;
		ray->sidedist_x = (ray->map_x + 1.0 - player->pos_x) * ray->deltadist_x;
	}
	if (ray->raydir_y < 0)
	{
		ray->step_y = -1;
		ray->sidedist_y = (player->pos_y - ray->map_y) * ray->deltadist_y;
	}
	else
	{
		ray->step_y = 1;
		ray->sidedist_y = (ray->map_y + 1.0 - player->pos_y) * ray->deltadist_y;
	}
}

/*

OG DDA

	c'est une boucle qui se demande quand on fait toucher une prochaine case
		Elle peut être en x ou y.


	side = 0 -> mur vertical | EST OUEST
	side = 1 -> mur horizontal | NORD SUD

	l'avant dernier if est une sécurité pour éviter de sortir de la map -> evite le segfault
	le dernier if verifie si on a touché un mur et change le bool
*/

static void og_dda(t_data *data, t_ray *ray)
{
	bool	is_hit;

	is_hit = false;
	while (is_hit == false)
	{
		if (ray->sidedist_x < ray->sidedist_y)
		{
			ray->sidedist_x += ray->deltadist_x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->sidedist_y += ray->deltadist_y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		if (ray->map_y < 0 || ray->map_x < 0 || ray->map_y >= data->map_info.height || ray->map_x >= data->map_info.width)
			break ;
		if (data->map[ray->map_y][ray->map_x] == '1')
			is_hit = true;
	}
	
}


/*
	Calcul de la hauteur du mur à afficher
		-> en fonction de la distance perpendiculaire entre le plan caméra et le mur touché
			-> permet de garder les murs droits
		-> calcul des positions de début et de fin pour le dessin vertical du mur
		-> calcul de la position exacte du mur touché (coordonnée x ou y en fonction du côté touché)	
*/

static void scaling_height(t_ray *ray, t_player *player, t_data *data)
{
	if (ray->side == 0)
		ray->perpWallDist = (ray->map_x - player->pos_x + (1 - ray->step_x) / 2) / ray->raydir_x;
	else
		ray->perpWallDist = (ray->map_y - player->pos_y + (1 - ray->step_y) / 2) / ray->raydir_y;
	if (ray->perpWallDist == 0)
		ray->perpWallDist = 0.1;
	ray->line_height = (int) (data->win_height / ray->perpWallDist);
	ray->start_draw = -(ray->line_height / 2) + data->win_height / 2;
	if (ray->start_draw < 0)
		ray->start_draw = 0;
	ray->end_draw = ray->line_height / 2 + data->win_height / 2;
	if (ray->end_draw >= data->win_height)
		ray->end_draw = data->win_height - 1;
	if (ray->side == 0)
		ray->wall_x = player->pos_y + ray->perpWallDist * ray->raydir_y;
	else
		ray->wall_x = player->pos_x + ray->perpWallDist * ray->raydir_x; //coordonnée exacte du mur touché
	ray->wall_x -= floor((ray->wall_x));
}

int actual_raycasting(t_player *player, t_data *data)
{
	t_ray	ray;
	int		x;

	x = 0;
	while (x < data->win_width)
	{
		initialization_raycast(x, &ray, player);
		setting_dda(&ray, player);
		og_dda(data, &ray);
		scaling_height(&ray, player, data);
		new_texture(data, &ray, &data->texture_info, x);
		x++;
	}
	return (0);
}