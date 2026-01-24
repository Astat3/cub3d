/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roazouan <roazouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 20:17:06 by roazouan          #+#    #+#             */
/*   Updated: 2026/01/23 17:39:55 by roazouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cube3d.h"

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
			if (map[i][j] != '1' && map[i][j] != '0' && map[i][j] != ' '
				&& map[i][j] != 'N' && map[i][j] != 'S' && map[i][j] != 'E'
				&& map[i][j] != 'W')
				(exit(ERRORS), printf("Error: Invalid character '%c' in map.\n", map[i][j]));
			if (map[i][j] == 'N' || map[i][j] == 'S' || map[i][j] == 'E'
				|| map[i][j] == 'W')
			{
				parsing->player_start++;
				parsing->player_start_x = j;
				parsing->player_start_y = i;
			}
		}
	}
	if (parsing->player_start != 1)
		(exit(ERRORS), printf("Error: There must be exactly one player start position.\n"));
}

static void	check_top_bottom(char **map, int len)
{
    int	i;

    i = 0;
    while (map[0][i])
    {
        if (map[0][i] != '1')
            (printf("Error: Top wall is not closed.\n"), exit(ERRORS));
        i++;
    }
    i = 0;
    while (map[len - 1][i])
    {
        if (map[len - 1][i] != '1')
            (printf("Error: Bottom wall is not closed.\n"), exit(ERRORS));
        i++;
    }
}

static void	check_walls(char **map)
{
    int	i;
    int	len;

    if (!map || !map[0])
        (printf("Error: Empty map.\n"), exit(ERRORS));
    len = ft_arraylen(map);
    if (len < 3)
        (printf("Error: Map too small.\n"), exit(ERRORS));
    check_top_bottom(map, len);
    i = 0;
    while (i < len)
    {
        if (!map[i] || ft_strlen(map[i]) == 0)
            (printf("Error: Empty line in map.\n"), exit(ERRORS));
        if (map[i][0] != '1')
            (printf("Error: Left wall is not closed.\n"), exit(ERRORS));
        if (map[i][ft_strlen(map[i]) - 1] != '1')
            (printf("Error: Right wall is not closed.\n"), exit(ERRORS));
        i++;
    }
}

static void	check_map(char **map, t_parsing *parsing)
{
	parsing->player_start = 0;
	check_wrong_char(map, parsing);
	check_walls(map);
	parsing->cp_map = ft_calloc(ft_arraylen(parsing->map) + 1, sizeof(char *));
	parsing->cp_map = ft_arraydup(parsing->map, parsing->cp_map);
	printf("perso = [%c]\n", parsing->map[parsing->player_start_y]
		[parsing->player_start_x]);
	check_map_flood(parsing, parsing->player_start_y, parsing->player_start_x, 
		parsing->map[parsing->player_start_y][parsing->player_start_x]);
	for (int i = 0; parsing->cp_map[i]; i++)
		printf("cp_map[%d] = [%s]\n", i, parsing->cp_map[i]);
	if (chrmap(parsing->cp_map) == 0)
	{
		printf("Error: Character is not enclosed by walls.\n");
		exit(ERRORS);
	}
	normalize_map(parsing->map);
	for (int i = 0; parsing->map[i]; i++)
		printf("[%s]\n", parsing->map[i]);
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