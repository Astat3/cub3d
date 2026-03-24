/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roazouan <roazouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 20:17:06 by roazouan          #+#    #+#             */
/*   Updated: 2026/03/17 00:00:00 by roazouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

static void	check_wrong_char(char **map, t_parsing *parsing)
{
	int	i;
	int	j;

	i = -1;
	while (map[++i])
	{
		j = -1;
		while (map[i][++j])
		{
			if (!ft_strchr("01 \tNSEW", map[i][j]))
				(printf("Error: Invalid character '%c' in map.\n", map[i][j]),
					free_parsing(parsing), exit(ERRORS));
			if (ft_strchr("NSEW", map[i][j]))
			{
				parsing->player_start++;
				parsing->player_start_x = j;
				parsing->player_start_y = i;
			}
		}
	}
	if (parsing->player_start != 1)
		(printf("Error: There must be exactly one player start position.\n"),
			free_parsing(parsing), exit(ERRORS));
}

static void	check_top_bottom(char **map, int len, t_parsing *parsing)
{
	int	i;

	i = 0;
	while (map[0][i])
	{
		if (map[0][i] != '1' && map[0][i] != ' ' && map[0][i] != '\t')
			(printf("Error: Top wall is not closed.\n"),
				free_parsing(parsing), exit(ERRORS));
		i++;
	}
	i = 0;
	while (map[len - 1][i])
	{
		if (map[len - 1][i] != '1' && map[len - 1][i] != ' '
			&& map[len - 1][i] != '\t')
			(printf("Error: Bottom wall is not closed.\n"),
				free_parsing(parsing), exit(ERRORS));
		i++;
	}
}

static void	check_walls(char **map, t_parsing *parsing)
{
	int	i;
	int	len;

	if (!map || !map[0])
		(printf("Error: Empty map.\n"), free_parsing(parsing), exit(ERRORS));
	len = ft_arraylen(map);
	if (len < 3)
		(printf("Error: Map too small.\n"),
			free_parsing(parsing), exit(ERRORS));
	check_top_bottom(map, len, parsing);
	i = 0;
	while (i < len)
	{
		if (!map[i] || ft_strlen(map[i]) == 0)
			(printf("Error: Empty line in map.\n"),
				free_parsing(parsing), exit(ERRORS));
		i++;
	}
}

static void	check_map(char **map, t_parsing *parsing)
{
	parsing->player_start = 0;
	check_wrong_char(map, parsing);
	check_walls(map, parsing);
	free_tab(parsing->cp_map);
	parsing->cp_map = ft_calloc(ft_arraylen(parsing->map) + 1, sizeof(char *));
	parsing->cp_map = ft_arraydup(parsing->map, parsing->cp_map);
	check_map_flood(parsing, parsing->player_start_y, parsing->player_start_x,
		parsing->map[parsing->player_start_y][parsing->player_start_x]);
	if (chrmap(parsing->cp_map) == 0)
		(printf("Error: Character is not enclosed by walls.\n"),
			free_parsing(parsing), exit(ERRORS));
	free_tab(parsing->cp_map);
	parsing->cp_map = NULL;
	normalize_map(parsing->map);
}

int	check_parsing(t_parsing *parsing)
{
	if (!parsing->no_texture || !parsing->so_texture
		|| !parsing->we_texture || !parsing->ea_texture
		|| !parsing->floor_color || !parsing->ceiling_color || !parsing->map)
	{
		printf("Error: Missing texture or color information.\n");
		return (ERRORS);
	}
	check_map(parsing->map, parsing);
	return (0);
}
