/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roazouan <roazouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 18:28:42 by roazouan          #+#    #+#             */
/*   Updated: 2026/03/17 00:00:00 by roazouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

static int	is_valid_pos(t_parsing *parsing, int y, int x)
{
	int	max_y;

	max_y = ft_arraylen(parsing->cp_map);
	if (y < 0 || y >= max_y || x < 0)
		return (0);
	if (!parsing->cp_map[y])
		return (0);
	if (x >= (int)ft_strlen(parsing->cp_map[y]))
		return (0);
	return (1);
}

static int	can_flood(t_parsing *parsing, int y, int x, char perso)
{
	if (!is_valid_pos(parsing, y, x))
		return (-1);
	if (parsing->cp_map[y][x] == ' ')
		return (-1);
	if (parsing->cp_map[y][x] == '1' || parsing->cp_map[y][x] == 'F')
		return (0);
	if (parsing->cp_map[y][x] == perso)
		return (0);
	return (1);
}

static void	flood_direction(t_parsing *p, int y, int x, char perso)
{
	if (can_flood(p, y + 1, x, perso) == -1
		|| can_flood(p, y - 1, x, perso) == -1
		|| can_flood(p, y, x + 1, perso) == -1
		|| can_flood(p, y, x - 1, perso) == -1)
	{
		printf("Error: Character is not enclosed by walls.\n");
		free_parsing(p);
		exit(ERRORS);
	}
	if (can_flood(p, y + 1, x, perso) == 1)
		check_map_flood(p, y + 1, x, perso);
	if (can_flood(p, y - 1, x, perso) == 1)
		check_map_flood(p, y - 1, x, perso);
	if (can_flood(p, y, x + 1, perso) == 1)
		check_map_flood(p, y, x + 1, perso);
	if (can_flood(p, y, x - 1, perso) == 1)
		check_map_flood(p, y, x - 1, perso);
}

void	check_map_flood(t_parsing *parsing, int new_y, int new_x, char perso)
{
	if (!is_valid_pos(parsing, new_y, new_x))
	{
		printf("Error: Character is not enclosed by walls.\n");
		free_parsing(parsing);
		exit(ERRORS);
	}
	if (parsing->cp_map[new_y][new_x] == ' ')
	{
		printf("Error: Character is not enclosed by walls.\n");
		free_parsing(parsing);
		exit(ERRORS);
	}
	if (parsing->cp_map[new_y][new_x] == '1'
		|| parsing->cp_map[new_y][new_x] == 'F')
		return ;
	parsing->cp_map[new_y][new_x] = 'F';
	flood_direction(parsing, new_y, new_x, perso);
}

int	chrmap(char **map)
{
	int	i;
	int	j;

	i = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			if (map[i][j] == 'N' || map[i][j] == 'S'
				|| map[i][j] == 'E' || map[i][j] == 'W')
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}
