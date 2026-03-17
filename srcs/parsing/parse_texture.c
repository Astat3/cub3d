/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_texture.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roazouan <roazouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 00:00:00 by roazouan          #+#    #+#             */
/*   Updated: 2026/03/17 00:00:00 by roazouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

static char	*skip_spaces(char *str)
{
	while (*str == ' ' || *str == '\t')
		str++;
	return (str);
}

static int	is_map_line(const char *line)
{
	int	i;

	if (!line || line[0] == '\0')
		return (0);
	i = 0;
	while (line[i])
	{
		if (line[i] != '0' && line[i] != '1' && line[i] != ' '
			&& line[i] != 'N' && line[i] != 'S'
			&& line[i] != 'E' && line[i] != 'W')
			return (0);
		i++;
	}
	return (1);
}

static int	parse_texture_line(t_parsing *parsing, char *line)
{
	if (ft_strncmp(line, "NO", 2) == 0
		&& (line[2] == ' ' || line[2] == '\t' || line[2] == '\0'))
		set_texture(parsing, &parsing->no_texture, skip_spaces(line + 2), "NO");
	else if (ft_strncmp(line, "SO", 2) == 0
		&& (line[2] == ' ' || line[2] == '\t' || line[2] == '\0'))
		set_texture(parsing, &parsing->so_texture, skip_spaces(line + 2), "SO");
	else if (ft_strncmp(line, "WE", 2) == 0
		&& (line[2] == ' ' || line[2] == '\t' || line[2] == '\0'))
		set_texture(parsing, &parsing->we_texture, skip_spaces(line + 2), "WE");
	else if (ft_strncmp(line, "EA", 2) == 0
		&& (line[2] == ' ' || line[2] == '\t' || line[2] == '\0'))
		set_texture(parsing, &parsing->ea_texture, skip_spaces(line + 2), "EA");
	else if (line[0] == 'F'
		&& (line[1] == ' ' || line[1] == '\t' || line[1] == '\0'))
		set_color(parsing, &parsing->floor_color, skip_spaces(line + 1), "F");
	else if (line[0] == 'C'
		&& (line[1] == ' ' || line[1] == '\t' || line[1] == '\0'))
		set_color(parsing, &parsing->ceiling_color, skip_spaces(line + 1), "C");
	else
		return (0);
	return (1);
}

static void	process_texture_line(t_parsing *parsing, int fd, int *i)
{
	char	*line;
	char	*trimmed;

	line = read_line(fd, parsing);
	if (!line)
		(printf("Error: Missing texture or color.\n"),
			free_parsing(parsing), exit(ERRORS));
	trimmed = ft_strtrim(line, " \t\n");
	free(line);
	if (!trimmed)
		(free_parsing(parsing), exit(ERRORS));
	if (trimmed[0] == '\0')
		return (free(trimmed));
	if (parse_texture_line(parsing, trimmed))
		(*i)++;
	else if (is_map_line(trimmed))
		(printf("Error: Map before all textures.\n"),
			free_parsing(parsing), exit(ERRORS));
	else
		(printf("Error: Invalid identifier in texture section.\n"),
			free_parsing(parsing), exit(ERRORS));
	free(trimmed);
}

void	init_textures(t_parsing *parsing, int fd)
{
	int	i;

	if (!parsing)
		(printf("Error: Parsing is NULL.\n"), free_parsing(parsing),
			exit(ERRORS));
	if (fd < 0)
		(printf("Error: Invalid file descriptor.\n"),
			free_parsing(parsing), exit(ERRORS));
	i = 0;
	while (i < 6)
		process_texture_line(parsing, fd, &i);
}
