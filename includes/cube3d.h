/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adamgallot <adamgallot@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 14:48:11 by adamgallot        #+#    #+#             */
/*   Updated: 2026/01/20 13:19:51 by adamgallot       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef CUBE3D_H
# define CUBE3D_H

#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "stdbool.h"
#include "string.h"
#include "unistd.h"
#include "fcntl.h"
//#include "mlx.h"
#include "handler.h"

# define WIDTH  1024
# define HEIGHT 720
# define move_speed 0.1
# define rot_speed 0.05

typedef enum e_direction
{
	NORTH,
	SOUTH,
	EAST,
	WEST
}   t_direction;

typedef enum e_macro
{
	SUCCESS,
	ERRORS,
	EXIT
}   t_macro;


typedef struct s_ray
{
	int     map_x;
	int     map_y;
	double camera_x;
	double raydir_x;
	double raydir_y;
	int     step_x;
	int     step_y;
	double sidedist_x;
	double sidedist_y;
	double deltadist_x;
	double deltadist_y;
	int     side; // rayon mur vertical ou horizontal -> utile dans le choix de la texture
	double perpWallDist; // distance entre le plan cam et le mur 
	// MAIS C'EST LA DISTANCE PERPENDICULAIRE -> pour garder les murs droits
	int side;
	
}   t_ray;

typedef struct s_img
{
	void	*img;
	int		*addr;
	int		pixel_bits;
	int		size_line;
	int		endian;
}	t_img;


// tempororaire stv changer des trucs mais selon mes recherches c'est le plus effiace
typedef struct mapinfotemp
{
	int     fd;
	int     line_count; // compteur pour gnl
	int     height;
	int     width;
	char    **file;
	char    *path;
	bool     end_of_map; //detecte la fin de la map | si elle est invalide
} t_mapinfotemp;

typedef struct s_player
{
	char  	get_dir; // 'N', 'S', 'E', 'W'
	double	pos_x; // player's position x
	double	pos_y;
	double	dir_x; // player's direction vector x
	double	dir_y;
	double	plane_x; // camera plane x | must be perpendicular to dir_x and dir_y
	double	plane_y;
	bool    has_moved;
	int     move_x;
	int     move_y;
	int     rotation;
	
}    t_player;
/*
t_ray
t_player
t_map_info


*/

typedef struct s_data
{
	void	*mlx;
	void	*win;
	int		win_width;
	int		win_height;
	char	**map;
	t_player player;
	t_ray   ray;
	t_mapinfotemp map_info;
	// texture_pixel.. 
} t_data;


// init
void	init_data(t_data *data);
void	create_img(t_img *img);
void	init_raycast(t_ray *ray);

//handling
void	listen_input(t_data *data);

//exit
void	quit_code(t_data *data);

//ray
int actual_raycasting(t_player *player, t_data *data);

#endif