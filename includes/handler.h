/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adamgallot <adamgallot@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 18:11:13 by adamgallot        #+#    #+#             */
/*   Updated: 2026/01/19 18:11:33 by adamgallot       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef KEYS_H
# define KEYS_H

# ifdef __APPLE__
#  define XK_Escape     53
#  define XK_Left       123
#  define XK_Right      124
#  define XK_Down       125
#  define XK_Up         126
#  define XK_w          13
#  define XK_a          0
#  define XK_s          1
#  define XK_d          2

# else
#  define XK_Escape     65307
#  define XK_Left       65361
#  define XK_Right      65363
#  define XK_Down       65364
#  define XK_Up         65362
#  define XK_w          119
#  define XK_a          97
#  define XK_s          115
#  define XK_d          100
# endif

#endif