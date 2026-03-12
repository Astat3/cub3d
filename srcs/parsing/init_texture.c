/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_texture.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roazouan <roazouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 19:57:52 by roazouan          #+#    #+#             */
/*   Updated: 2026/01/24 17:06:39 by roazouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

static void	exit_error(int fd, const char *msg, t_parsing *parsing, char *to_free)
{
    if (msg)
        printf("%s", msg);
    if (fd >= 0)
        close(fd);
    if (to_free)
        free(to_free);
    if (parsing)
        free_parsing(parsing);
    exit(ERRORS);
}

static char	*ft_strjoin_char(char *str, char c)
{
    char	*new;
    int		len;
    int		i;

    len = ft_strlen(str);
    new = malloc(sizeof(char) * (len + 2));
    if (!new)
        return (NULL);
    i = 0;
    while (i < len)
    {
        new[i] = str[i];
        i++;
    }
    new[i] = c;
    new[i + 1] = '\0';
    free(str);
    return (new);
}

char	*read_line(int fd, t_parsing *parsing)
{
    char	*line;
    char	c;
    int		ret;

    line = NULL;
    ret = read(fd, &c, 1);
    if (ret < 0)
        exit_error(fd, "Error: Failed to read file.\n", parsing, line);
    while (ret > 0)
    {
        line = ft_strjoin_char(line, c);
        if (!line)
            exit_error(fd, "Error: Memory allocation failed.\n", parsing, NULL);
        if (c == '\n')
            return (line);
        ret = read(fd, &c, 1);
        if (ret < 0)
            exit_error(fd, "Error: Failed to read file.\n", parsing, line);
    }
    return (line);
}

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

int	has_suffix(const char *str, const char *suffix)
{
    size_t	len;
    size_t	suffix_len;

    if (!str || !suffix)
        return (0);
    len = (size_t)ft_strlen(str);
    suffix_len = (size_t)ft_strlen(suffix);
    if (len < suffix_len)
        return (0);
    return (ft_strncmp(str + len - suffix_len, suffix, suffix_len) == 0);
}

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

static void	set_texture(t_parsing *parsing, char **dst, const char *value, const char *id, int fd, char *trimmed)
{
    int	tex_fd;

    if (*dst)
    {
        printf("Error: Duplicate %s texture.\n", id);
        exit_error(fd, NULL, parsing, trimmed);
    }
    if (!value || value[0] == '\0')
    {
        printf("Error: Missing %s texture path.\n", id);
        exit_error(fd, NULL, parsing, trimmed);
    }
    if (has_whitespace(value))
    {
        printf("Error: Invalid %s texture path.\n", id);
        exit_error(fd, NULL, parsing, trimmed);
    }
    if (!has_suffix(value, ".xpm"))
    {
        printf("Error: %s texture must be a .xpm file.\n", id);
        exit_error(fd, NULL, parsing, trimmed);
    }
    tex_fd = open(value, O_RDONLY);
    if (tex_fd < 0)
    {
        printf("Error: %s texture file not found.\n", id);
        exit_error(fd, NULL, parsing, trimmed);
    }
    close(tex_fd);
    *dst = ft_strdup(value);
    if (!*dst)
        exit_error(fd, "Error: Memory allocation failed.\n", parsing, trimmed);
}

static void	set_color(t_parsing *parsing, char **dst, const char *value, const char *id, int fd, char *trimmed)
{
    if (*dst)
    {
        printf("Error: Duplicate %s color.\n", id);
        exit_error(fd, NULL, parsing, trimmed);
    }
    if (!value || value[0] == '\0')
    {
        printf("Error: Missing %s color value.\n", id);
        exit_error(fd, NULL, parsing, trimmed);
    }
    if (!is_valid_color(value))
    {
        printf("Error: Invalid %s color format.\n", id);
        exit_error(fd, NULL, parsing, trimmed);
    }
    *dst = ft_strdup(value);
    if (!*dst)
        exit_error(fd, "Error: Memory allocation failed.\n", parsing, trimmed);
}

static int	parse_texture_line(t_parsing *parsing, char *line, int fd)
{
    if (ft_strncmp(line, "NO", 2) == 0
        && (line[2] == ' ' || line[2] == '\t' || line[2] == '\0'))
        set_texture(parsing, &parsing->no_texture, skip_spaces(line + 2), "NO", fd, line);
    else if (ft_strncmp(line, "SO", 2) == 0
        && (line[2] == ' ' || line[2] == '\t' || line[2] == '\0'))
        set_texture(parsing, &parsing->so_texture, skip_spaces(line + 2), "SO", fd, line);
    else if (ft_strncmp(line, "WE", 2) == 0
        && (line[2] == ' ' || line[2] == '\t' || line[2] == '\0'))
        set_texture(parsing, &parsing->we_texture, skip_spaces(line + 2), "WE", fd, line);
    else if (ft_strncmp(line, "EA", 2) == 0
        && (line[2] == ' ' || line[2] == '\t' || line[2] == '\0'))
        set_texture(parsing, &parsing->ea_texture, skip_spaces(line + 2), "EA", fd, line);
    else if (line[0] == 'F'
        && (line[1] == ' ' || line[1] == '\t' || line[1] == '\0'))
        set_color(parsing, &parsing->floor_color, skip_spaces(line + 1), "F", fd, line);
    else if (line[0] == 'C'
        && (line[1] == ' ' || line[1] == '\t' || line[1] == '\0'))
        set_color(parsing, &parsing->ceiling_color, skip_spaces(line + 1), "C", fd, line);
    else
        return (0);
    return (1);
}

void init_textures(t_parsing *parsing, int fd)
{
    int		i;
    char	*line;
    char	*trimmed;

    if (!parsing)
        exit_error(fd, "Error: Parsing is NULL.\n", NULL, NULL);
    if (fd < 0)
        exit_error(fd, "Error: Invalid file descriptor.\n", parsing, NULL);
    i = 0;
    while (i < 6)
    {
        line = read_line(fd, parsing);
        if (!line)
            exit_error(fd, "Error: Missing texture or color information.\n", parsing, NULL);
        trimmed = ft_strtrim(line, " \t\n");
        free(line);
        if (!trimmed)
            exit_error(fd, "Error: Memory allocation failed.\n", parsing, NULL);
        if (trimmed[0] == '\0')
        {
            free(trimmed);
            continue;
        }
        if (parse_texture_line(parsing, trimmed, fd))
            i++;
        else if (is_map_line(trimmed))
            exit_error(fd, "Error: Map encountered before all textures.\n", parsing, trimmed);
        else
            exit_error(fd, "Error: Invalid identifier in texture section.\n", parsing, trimmed);
        free(trimmed);
    }
}
