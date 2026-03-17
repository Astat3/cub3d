/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_texture.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roazouan <roazouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 19:57:52 by roazouan          #+#    #+#             */
/*   Updated: 2026/03/17 00:00:00 by roazouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

static int	parse_color_value(const char *str, int *i)
{
	int	value;
	int	digits;

	value = 0;
	digits = 0;
	while (str[*i] == ' ' || str[*i] == '\t')
		(*i)++;
	while (ft_isdigit(str[*i]))
	{
		value = value * 10 + (str[*i] - '0');
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

static int	is_valid_color(const char *str)
{
	int	i;

	i = 0;
	if (parse_color_value(str, &i) < 0 || str[i] != ',')
		return (0);
	i++;
	if (parse_color_value(str, &i) < 0 || str[i] != ',')
		return (0);
	i++;
	if (parse_color_value(str, &i) < 0)
		return (0);
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	return (str[i] == '\0');
}

static int	has_whitespace(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == ' ' || str[i] == '\t')
			return (1);
		i++;
	}
	return (0);
}

void	set_texture(t_parsing *parsing, char **dst, const char *value,
			const char *id)
{
	int	tex_fd;

	if (*dst)
		(printf("Error: Duplicate %s texture.\n", id),
			free_parsing(parsing), exit(ERRORS));
	if (!value || value[0] == '\0')
		(printf("Error: Missing %s texture path.\n", id),
			free_parsing(parsing), exit(ERRORS));
	if (has_whitespace(value))
		(printf("Error: Invalid %s texture path.\n", id),
			free_parsing(parsing), exit(ERRORS));
	if (!has_suffix(value, ".xpm"))
		(printf("Error: %s texture must be a .xpm file.\n", id),
			free_parsing(parsing), exit(ERRORS));
	tex_fd = open(value, O_RDONLY);
	if (tex_fd < 0)
		(printf("Error: %s texture file not found.\n", id),
			free_parsing(parsing), exit(ERRORS));
	close(tex_fd);
	*dst = ft_strdup(value);
	if (!*dst)
		(free_parsing(parsing), exit(ERRORS));
}

void	set_color(t_parsing *parsing, char **dst, const char *value,
			const char *id)
{
	if (*dst)
		(printf("Error: Duplicate %s color.\n", id),
			free_parsing(parsing), exit(ERRORS));
	if (!value || value[0] == '\0')
		(printf("Error: Missing %s color value.\n", id),
			free_parsing(parsing), exit(ERRORS));
	if (!is_valid_color(value))
		(printf("Error: Invalid %s color format.\n", id),
			free_parsing(parsing), exit(ERRORS));
	*dst = ft_strdup(value);
	if (!*dst)
		(free_parsing(parsing), exit(ERRORS));
}
