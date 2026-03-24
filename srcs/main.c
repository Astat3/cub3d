/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roazouan <roazouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 21:08:27 by roazouan          #+#    #+#             */
/*   Updated: 2026/02/07 00:00:00 by roazouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cube3d.h"

static void	detach_from_parsing(t_parsing *parsing)
{
	parsing->no_texture = NULL;
	parsing->so_texture = NULL;
	parsing->we_texture = NULL;
	parsing->ea_texture = NULL;
	parsing->floor_color = NULL;
	parsing->ceiling_color = NULL;
	parsing->map = NULL;
}

static int	parsing_func(char *file, t_parsing *parsing)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0 || !parsing)
		return (printf("Error: Cannot open file: %s\n", file), ERRORS);
	if (has_suffix(file, ".cub") == 0)
	{
		printf("Error: File must have a .cub extension.\n");
		close(fd);
		return (ERRORS);
	}
	parsing->fd = fd;
	init_textures(parsing, fd);
	init_map(parsing, fd);
	check_parsing(parsing);
	return (SUCCESS);
}

static int	init_game(t_data *data, t_parsing *parsing)
{
	init_data(data);
	if (copy_parsing_to_data(data, parsing) != SUCCESS)
		return (ERRORS);
	init_player_dir(data, parsing);
	detach_from_parsing(parsing);
	if (init_mlx(data) != SUCCESS)
		return (ERRORS);
	if (init_textures_img(data) != SUCCESS)
		return (ERRORS);
	if (init_frame_img(data) != SUCCESS)
		return (ERRORS);
	return (SUCCESS);
}

int	main(int ac, char **av, char **env)
{
	t_parsing	*parsing;
	t_data		data;

	(void)env;
	if (ac != 2)
		return (printf("Usage: %s <map.cub>\n", av[0]), ERRORS);
	parsing = ft_calloc(1, sizeof(t_parsing));
	if (!parsing)
		return (printf("Error: Memory allocation failed.\n"), ERRORS);
	if (parsing_func(av[1], parsing) != SUCCESS)
		return (free_parsing(parsing), ERRORS);
	if (init_game(&data, parsing) != SUCCESS)
	{
		free_data(&data);
		return (free_parsing(parsing), ERRORS);
	}
	free_parsing(parsing);
	listen_input(&data);
	render_frame(&data);
	mlx_loop(data.mlx);
	return (0);
}
