/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adamgallot <adamgallot@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 14:48:11 by adamgallot        #+#    #+#             */
/*   Updated: 2026/01/19 19:42:35 by adamgallot       ###   ########.fr       */
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


typedef struct s_img
{
	void	*img;
	int		*addr;
	int		pixel_bits;
	int		size_line;
	int		endian;
}	t_img;


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
} t_data;


//handling
void	listen_input(t_data *data);

//exit
void	quit_code(t_data *data);

#endif