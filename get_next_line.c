/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: duk <duk@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 15:39:11 by duk               #+#    #+#             */
/*   Updated: 2026/10/13 18:17:59 by duk              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/*
It keeps a static char *extra, so any characters that have been
read but not yet returned survive between calls. It checks for invalid
arguments, searches the saved data for a newline, and keeps reading and
appending chunks until it finds one. It then delegates line extraction to
sep_extra(). If a read returns an empty string at EOF,
it delegates to endfile_check().
*/

char	*get_next_line(int fd)
{
	static char	*extra;
	char		*buf;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	while (find_new_line(extra) == -1)
	{
		buf = reading_ft(fd, BUFFER_SIZE);
		if (buf == NULL)
		{
			free(extra);
			extra = NULL;
			return (NULL);
		}
		if (buf[0] == '\0')
			return (endfile_check(buf, &extra));
		else
		{
			extra = join_str(extra, buf);
			free(buf);
			if (extra == NULL)
				return (NULL);
		}
	}
	return (sep_extra(&extra));
}

/*
This function splits the saved string into two pieces:
the line to return and the characters after the newline.
It calls build_new_line() to create the returned line and
build_new_extra() to create the remainder. It frees the old
buffer and updates *extra_p to point to the remainder.
If either allocation fails, it frees the temporary allocations
and clears the saved buffer.
*/

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
This function handles the EOF case, which is detected when
read() returns zero bytes and reading_ft() therefore returns
an empty string. It frees that empty buffer. If extra still
contains text, it returns that text as the final line and clears
the saved pointer. If no text remains, it returns NULL.
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

/*
This function creates a new string by concatenating the previously
saved characters (extra) with the newly read chunk (buf). It frees
the old extra after copying it into the new allocation. The caller
frees buf after joining. If allocation fails, join_str() frees
extra and returns NULL.
*/

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
