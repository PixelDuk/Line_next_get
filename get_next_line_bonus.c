/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: duk <duk@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 15:39:11 by duk               #+#    #+#             */
/*   Updated: 2026/10/13 18:03:18 by duk              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*get_next_line(int fd)
{
	static char	*extra[FD_LIMIT];
	char		*buf;

	if (fd < 0 || fd >= 1024 || BUFFER_SIZE <= 0)
		return (NULL);
	while (find_new_line(extra[fd]) == -1)
	{
		buf = reading_ft(fd, BUFFER_SIZE);
		if (buf == NULL)
		{
			free(extra[fd]);
			extra[fd] = NULL;
			return (NULL);
		}
		if (buf[0] == '\0')
			return (endfile_check(buf, &extra[fd]));
		else
		{
			extra[fd] = join_str(extra[fd], buf);
			free(buf);
			if (extra[fd] == NULL)
				return (NULL);
		}
	}
	return (sep_extra(&extra[fd]));
}

char	*sep_extra(char **extra_p)
{
	char	*tmp_new_line;
	char	*tmp_new_extra;

	tmp_new_line = build_new_line(*extra_p);
	tmp_new_extra = build_new_extra(*extra_p);
	if (tmp_new_line == NULL || tmp_new_extra == NULL)
	{
		free(tmp_new_line);
		free(tmp_new_extra);
		free(*extra_p);
		*extra_p = NULL;
		return (NULL);
	}
	free(*extra_p);
	*extra_p = tmp_new_extra;
	return (tmp_new_line);
}

/*
the function is used only when the end of the file comes
where it checks if there is anything left and frees the
buffer and returns the last few char within the buffer size
if there is any, if there is none then returns NULL
*/

char	*endfile_check(char *buf, char **extra_p)
{
	char	*tmp;

	tmp = *extra_p;
	free(buf);
	if (*extra_p == NULL || (*extra_p)[0] == '\0')
	{
		free(*extra_p);
		*extra_p = NULL;
		return (NULL);
	}
	*extra_p = NULL;
	return (tmp);
}

/*
reads form the file descriptor and returns the amount of
char from the input to an allocated string, when read hits
the last line of text the function will return '\0' as a string.
*/
char	*reading_ft(int fd, size_t buffer_size)
{
	ssize_t	read_len;
	char	*buf;

	buf = malloc ((buffer_size + 1) * sizeof(char));
	if (buf == NULL)
		return (NULL);
	read_len = read(fd, buf, buffer_size);
	if (read_len == -1)
	{
		free(buf);
		return (NULL);
	}
	buf[read_len] = '\0';
	return (buf);
}

char	*join_str(char *extra, char *buf)
{
	int		len_extra;
	int		i;
	char	*joined_str;

	len_extra = len_str(extra);
	joined_str = malloc((len_extra + (len_str(buf)) + 1) * sizeof(char));
	i = 0;
	if (joined_str == NULL)
	{
		free(extra);
		return (NULL);
	}
	while (i < len_extra)
	{
		joined_str[i] = extra[i];
		i++;
	}
	while (buf[i - len_extra] != '\0')
	{
		joined_str[i] = buf[i - len_extra];
		i++;
	}
	joined_str[i] = '\0';
	free(extra);
	return (joined_str);
}
