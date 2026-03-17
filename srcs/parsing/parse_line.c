/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roazouan <roazouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 00:00:00 by roazouan          #+#    #+#             */
/*   Updated: 2026/03/17 00:00:00 by roazouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

static char	*ft_strjoin_char(char *str, char c)
{
	char	*res;
	int		len;
	int		i;

	len = 0;
	if (str)
		len = ft_strlen(str);
	res = malloc(sizeof(char) * (len + 2));
	if (!res)
		return (NULL);
	i = 0;
	while (i < len)
	{
		res[i] = str[i];
		i++;
	}
	res[i] = c;
	res[i + 1] = '\0';
	free(str);
	return (res);
}

char	*read_line(int fd, t_parsing *parsing)
{
	char	*line;
	char	c;
	int		ret;

	line = NULL;
	ret = read(fd, &c, 1);
	if (ret < 0)
		(printf("Error: Failed to read file.\n"), free_parsing(parsing),
			exit(ERRORS));
	while (ret > 0)
	{
		line = ft_strjoin_char(line, c);
		if (!line)
			(free_parsing(parsing), exit(ERRORS));
		if (c == '\n')
			return (line);
		ret = read(fd, &c, 1);
		if (ret < 0)
			(printf("Error: Failed to read file.\n"), free_parsing(parsing),
				exit(ERRORS));
	}
	return (line);
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
