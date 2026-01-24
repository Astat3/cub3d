/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roazouan <roazouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 20:01:10 by roazouan          #+#    #+#             */
/*   Updated: 2026/01/21 15:49:38 by roazouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cube3d.h"

void normalize_map(char **map)
{
	int i;
	size_t	max_len;
	char	*new_line;

	max_len = 0;
	i = 0;
	while (map[i])
	{
		if ((int)ft_strlen(map[i]) > (int)max_len)
			max_len = ft_strlen(map[i]);
		i++;
	}
	i = 0;
	while (map[i])
	{
		if ((int)ft_strlen(map[i]) < (int)max_len)
		{
			new_line = ft_calloc(max_len + 1, sizeof(char));
			ft_memset(new_line, 'Z', max_len);
			ft_memcpy(new_line, map[i], (size_t)ft_strlen(map[i]));
			free(map[i]);
			map[i] = new_line;
		}
		i++;
	}
}

void init_map(t_parsing *parsing, int fd)
{
	char    *line;
	char    *map_str;

	map_str = NULL;
	line = read_line(fd);
	while (line)
	{
		if (ft_strlen(line) > 1 && (line[0] == '1' || line[0] == ' '))
			map_str = ft_strjoin(map_str, line);
		free(line);
		line = read_line(fd);
	}
	parsing->map = ft_split(map_str, '\n');
	free(map_str);
	close(fd);
}
