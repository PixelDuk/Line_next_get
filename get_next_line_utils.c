/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: duk <duk@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 12:51:28 by duk               #+#    #+#             */
/*   Updated: 2026/10/09 13:34:01 by duk              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/*
writing function for finding null position in the tmp string
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
copies the new line from the tmp string (s in this case)
there maybe lack of beauty as i had to manually add the newline
becaude libft cannot be used so (no calloc) and i wasnt sure if
do while can be use so this is the way i came up with :_  
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
