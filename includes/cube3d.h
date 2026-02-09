/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roazouan <roazouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 14:48:11 by adamgallot        #+#    #+#             */
/*   Updated: 2026/02/07 00:00:00 by roazouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUBE3D_H
# define CUBE3D_H

# include "stdio.h"
# include "libft.h"
# include "stdlib.h"
# include "math.h"
# include "stdbool.h"
# include "string.h"
# include "unistd.h"
# include "fcntl.h"
# include "mlx.h"
# include "handler.h"

# define WIDTH 1024
# define HEIGHT 720
# define MOVE_SPEED 0.08
# define ROT_SPEED 0.05

typedef enum e_macro
{
	SUCCESS,
	ERRORS,
	EXIT
}	t_macro;

typedef enum e_texture
{
	NO,
	SO,
	WE,
	EA,
	F,
	C
}	t_texture;

typedef struct s_parsing
{
	char	*no_texture;
	char	*so_texture;
	char	*we_texture;
	char	*ea_texture;
	char	*floor_color;
	char	*ceiling_color;
	char	**map;
	char	**cp_map;
	int		player_start;
	int		player_start_x;
	int		player_start_y;
}	t_parsing;

typedef struct s_img
{
	void	*img;
	int		*addr;
	int		pixel_bits;
	int		size_line;
	int		endian;
	int		width;
	int		height;
}	t_img;

typedef struct s_tex
{
	t_img	north;
	t_img	south;
	t_img	east;
	t_img	west;
	int		size;
}	t_tex;

typedef struct s_player
{
	char	get_dir;
	double	pos_x;
	double	pos_y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
	bool	has_moved;
	int		move_x;
	int		move_y;
	int		rotation;
}	t_player;

typedef struct s_ray
{
	double	camera_x;
	double	dir_x;
	double	dir_y;
	int		map_x;
	int		map_y;
	double	side_dist_x;
	double	side_dist_y;
	double	delta_dist_x;
	double	delta_dist_y;
	int		step_x;
	int		step_y;
	int		hit;
	int		side;
	double	perp_wall_dist;
	int		line_height;
	int		draw_start;
	int		draw_end;
	double	wall_x;
	int		tex_x;
	int		tex_y;
}	t_ray;

typedef struct s_data
{
	void		*mlx;
	void		*win;
	int			win_width;
	int			win_height;
	char		*no_texture;
	char		*so_texture;
	char		*we_texture;
	char		*ea_texture;
	char		*floor_color;
	char		*ceiling_color;
	int			floor_rgb;
	int			ceiling_rgb;
	char		**map;
	t_player	player;
	t_tex		textures;
	t_img		frame;
}	t_data;

int		ft_arraylen(char **array);
char	**ft_arraydup(char **src, char **dst);
int		has_suffix(const char *str, const char *suffix);

void	init_data(t_data *data);
int		copy_parsing_to_data(t_data *data, t_parsing *parsing);
int		init_mlx(t_data *data);
void	init_player_dir(t_data *data, t_parsing *parsing);
int		init_textures_img(t_data *data);
int		init_frame_img(t_data *data);

void	init_textures(t_parsing *parsing, int fd);
void	init_map(t_parsing *parsing, int fd);
char	*read_line(int fd);
int		check_parsing(t_parsing *parsing);
void	normalize_map(char **map);
void	check_map_flood(t_parsing *parsing, int new_y, int new_x, char perso);
int		chrmap(char **map);

void	listen_input(t_data *data);
int		game_loop(t_data *data);
void	update_player(t_data *data);
void	render_frame(t_data *data);
void	render_column(t_data *data, int x);
void	my_mlx_pixel_put(t_img *img, int x, int y, int color);

void	free_tab(char **tab);
void	free_data(t_data *data);
int		quit_code(t_data *data);

#endif
