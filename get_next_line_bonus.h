/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: duk <duk@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/11 18:20:49 by duk               #+#    #+#             */
/*   Updated: 2026/10/13 16:22:58 by duk              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <unistd.h>
# include <stdlib.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1

# endif

# ifndef FD_LIMIT
#  define FD_LIMIT 1024

# endif

char	*get_next_line(int fd);
char	*sep_extra(char **extra_p);
char	*endfile_check(char *buf, char **extra_p);
char	*reading_ft(int fd, size_t buffer_size);
char	*join_str(char *extra, char *buf);
int		find_new_line(char *s);
char	*build_new_line(char *s);
int		len_str(char	*s);
char	*build_new_extra(char *s);

#endif
