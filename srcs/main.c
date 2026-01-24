/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roazouan <roazouan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 21:08:27 by roazouan          #+#    #+#             */
/*   Updated: 2026/01/20 20:16:55 by roazouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cube3d.h"


int	main(int ac, char **av, char **env)
{
    (void)ac;
    (void)env;
	int fd;
	t_parsing	*parsing;

	parsing = ft_calloc(1, sizeof(t_parsing));
	fd = open(av[1], O_RDONLY);
    if (fd < 0)
        return (0);
	init_textures(parsing, fd);
	init_map(parsing, fd);
	check_parsing(parsing);
    return (0);
}
