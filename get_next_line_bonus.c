/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkara2 <tkara2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 00:07:09 by tkara2            #+#    #+#             */
/*   Updated: 2024/06/05 11:10:21 by tkara2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "get_next_line_bonus.h"

static char	*add_line_from_stash(char *buffer, char *stash)
{
	char	*line;

	line = ft_strjoin(stash, buffer);
	if (!line)
		return (free(stash), NULL);
	return (line);
}

static char	*read_file_add_stash(int fd, char *stash)
{
	int		read_byte;
	char	*buffer;

	if (!stash)
		stash = ft_calloc(1, 1);
	read_byte = 1;
	buffer = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (!buffer)
		return (free(stash), NULL);
	while (read_byte != 0)
	{
		read_byte = read(fd, buffer, BUFFER_SIZE);
		if (read_byte == -1)
			return (free(stash), free(buffer), NULL);
		buffer[read_byte] = 0;
		stash = add_line_from_stash(buffer, stash);
		if (!stash)
			return (free(stash), free(buffer), NULL);
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	return (free(buffer), stash);
}

static char	*get_line_from_stash(char *stash)
{
	size_t	i;
	char	*line;

	i = 0;
	if (!stash[i])
		return (NULL);
	while (stash[i] && stash[i] != '\n')
		i++;
	line = ft_calloc(i + 2, sizeof(char));
	if (!line)
		return (NULL);
	i = 0;
	while (stash[i] && stash[i] != '\n')
	{
		line[i] = stash[i];
		i++;
	}
	if (stash[i] && stash[i] == '\n')
		line[i++] = '\n';
	return (line);
}

static char	*remove_line_from_stash(char *stash)
{
	size_t	i;
	size_t	j;
	char	*new_stash;

	i = 0;
	j = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (!stash[i])
		return (free(stash), NULL);
	new_stash = ft_calloc((ft_strlen(stash) - i + 1), sizeof(char));
	if (!new_stash)
		return (free(stash), NULL);
	i++;
	while (stash[i])
		new_stash[j++] = stash[i++];
	return (free(stash), new_stash);
}

char	*get_next_line(int fd)
{
	static char	*stash[MAX_FD];
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || fd > MAX_FD)
		return (NULL);
	stash[fd] = read_file_add_stash(fd, stash[fd]);
	if (!stash[fd])
		return (NULL);
	line = get_line_from_stash(stash[fd]);
	stash[fd] = remove_line_from_stash(stash[fd]);
	return (line);
}
