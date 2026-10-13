/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: duk <duk@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 12:51:28 by duk               #+#    #+#             */
/*   Updated: 2026/10/13 18:13:44 by duk              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/*
Searches the string for the first \n and returns its index.
It returns -1 if the string is NULL or contains no newline.
get_next_line() uses this result to decide whether it needs to read more data.
*/

int	find_new_line(char *s)
{
	int	i;

	i = 0;
	if (s == NULL)
		return (-1);
	while (s[i] != '\0')
	{
		if (s[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

/*
Copies the characters from the beginning of s through the first newline
into a newly allocated string. The newline is included, and the returned
string is null-terminated. It is called by sep_extra() only after a
newline has been found.
*/
char	*build_new_line(char *s)
{
	char	*new_line;
	int		pos_newline;
	int		i;

	pos_newline = find_new_line(s);
	new_line = malloc(((pos_newline + 1) + 1) * sizeof(char));
	if (new_line == NULL)
		return (NULL);
	i = 0;
	while (s[i] != '\n')
	{
		new_line[i] = s[i];
		i++;
	}
	new_line[i] = '\n';
	i++;
	new_line[i] = '\0';
	return (new_line);
}

/*
finds the length of a string, if the string
is empty will return 0
*/

int	len_str(char	*s)
{
	int	i;

	i = 0;
	if (s == NULL)
		return (i);
	while (s[i] != '\0')
	{
		i++;
	}
	return (i);
}

/*
Copies the characters after the first newline into a new string.
These are the characters that belong to later lines and must be
kept for the next call. If there are no characters after the newline,
it creates an empty string.
*/

char	*build_new_extra(char *s)
{
	int		extra_len;
	int		len;
	int		pos_newline;
	int		i;
	char	*extra;

	len = len_str(s);
	pos_newline = find_new_line(s);
	extra_len = len - (pos_newline +1);
	i = 0;
	extra = malloc((extra_len + 1) * sizeof(char));
	if (extra == NULL)
		return (NULL);
	while (i < extra_len)
	{
		extra[i] = s[pos_newline + 1 + i];
		i++;
	}
	extra[i] = '\0';
	return (extra);
}
