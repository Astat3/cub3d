/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_color.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roazouan <roazouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 00:00:00 by roazouan          #+#    #+#             */
/*   Updated: 2026/03/17 00:00:00 by roazouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

static int	parse_color_component(const char *str, int *i)
{
	int	value;
	int	digits;

	value = 0;
	digits = 0;
	while (str[*i] == ' ' || str[*i] == '\t')
		(*i)++;
	while (ft_isdigit(str[*i]))
	{
		value = (value * 10) + (str[*i] - '0');
		if (value > 255)
			return (-1);
		(*i)++;
		digits++;
	}
	if (digits == 0)
		return (-1);
	while (str[*i] == ' ' || str[*i] == '\t')
		(*i)++;
	return (value);
}

static int	parse_rgb_color(const char *str, int *rgb)
{
	int	i;
	int	r;
	int	g;
	int	b;

	if (!str || !rgb)
		return (ERRORS);
	i = 0;
	r = parse_color_component(str, &i);
	if (r < 0 || str[i++] != ',')
		return (ERRORS);
	g = parse_color_component(str, &i);
	if (g < 0 || str[i++] != ',')
		return (ERRORS);
	b = parse_color_component(str, &i);
	if (b < 0)
		return (ERRORS);
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	if (str[i] != '\0')
		return (ERRORS);
	*rgb = (r << 16) | (g << 8) | b;
	return (SUCCESS);
}

int	copy_parsing_to_data(t_data *data, t_parsing *parsing)
{
	if (!data || !parsing)
		return (printf("Error: Parsing or data is NULL.\n"), ERRORS);
	if (!parsing->no_texture || !parsing->so_texture || !parsing->we_texture
		|| !parsing->ea_texture || !parsing->floor_color
		|| !parsing->ceiling_color || !parsing->map)
		return (printf("Error: Missing parsing information.\n"), ERRORS);
	data->no_texture = parsing->no_texture;
	data->so_texture = parsing->so_texture;
	data->we_texture = parsing->we_texture;
	data->ea_texture = parsing->ea_texture;
	data->floor_color = parsing->floor_color;
	data->ceiling_color = parsing->ceiling_color;
	data->map = parsing->map;
	if (parse_rgb_color(data->floor_color, &data->floor_rgb) != SUCCESS)
		return (printf("Error: Invalid floor color.\n"), ERRORS);
	if (parse_rgb_color(data->ceiling_color, &data->ceiling_rgb) != SUCCESS)
		return (printf("Error: Invalid ceiling color.\n"), ERRORS);
	return (SUCCESS);
}
