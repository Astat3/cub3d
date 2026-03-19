/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roazouan <roazouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 20:01:10 by roazouan          #+#    #+#             */
/*   Updated: 2026/02/07 00:00:00 by roazouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

void	normalize_map(char **map)
{
	int		i;
	int		max_len;
	char	*new_line;

	max_len = 0;
	i = 0;
	while (map[i])
	{
		if (ft_strlen(map[i]) > max_len)
			max_len = ft_strlen(map[i]);
		i++;
	}
	i = 0;
	while (map[i])
	{
		if (ft_strlen(map[i]) < max_len)
		{
			new_line = ft_calloc((size_t)max_len + 1, sizeof(char));
			ft_memset(new_line, 'Z', (size_t)max_len);
			ft_memcpy(new_line, map[i], (size_t)ft_strlen(map[i]));
			free(map[i]);
			map[i] = new_line;
		}
		i++;
	}
}

static int	is_map_line(char *line)
{
	if (!line || line[0] == '\n')
		return (0);
	if (line[0] == '1' || line[0] == '0' || line[0] == ' ')
		return (1);
	if (line[0] == 'N' || line[0] == 'S' || line[0] == 'E' || line[0] == 'W')
		return (1);
	return (0);
}

static char	*append_line(char *dst, char *line)
{
	char	*tmp;

	tmp = ft_strjoin(dst, line);
	free(dst);
	return (tmp);
}

static void	check_line_state(char *line, int *state, t_parsing *parsing)
{
	if (*state == 0)
	{
		if (line[0] != '\n' && is_map_line(line))
			*state = 1;
		else if (line[0] != '\n')
			(printf("Error: Invalid map content.\n"),
				free_parsing(parsing), exit(ERRORS));
	}
	else if (*state == 1)
	{
		if (line[0] == '\n')
			*state = 2;
		else if (!is_map_line(line))
			(printf("Error: Invalid map content.\n"),
				free_parsing(parsing), exit(ERRORS));
	}
	else if (*state == 2 && line[0] != '\n')
		(printf("Error: Trailing garbage after map.\n"),
			free_parsing(parsing), exit(ERRORS));
}

void	init_map(t_parsing *parsing, int fd)
{
	char	*line;
	char	*map_str;
	int		state;

	state = 0;
	map_str = ft_strdup("");
	line = read_line(fd, parsing);
	while (line)
	{
		check_line_state(line, &state, parsing);
		if (state == 1)
			map_str = append_line(map_str, line);
		free(line);
		line = read_line(fd, parsing);
	}
	close(fd);
	if (!map_str || map_str[0] == '\0')
		(free(map_str), free_parsing(parsing),
			printf("Error: Missing map.\n"), exit(ERRORS));
	parsing->map = ft_split(map_str, '\n');
	free(map_str);
	if (!parsing->map || !parsing->map[0])
		(free_parsing(parsing), printf("Error: Invalid map.\n"), exit(ERRORS));
}
