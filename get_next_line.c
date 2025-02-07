/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridomuka <ridomuka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 21:52:55 by ridomuka          #+#    #+#             */
/*   Updated: 2025/02/04 12:54:32 by ridomuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*extract_line(char *buffer)
{
	size_t	len;
	char	*line;

	len = 0;
	if (!buffer || buffer[0] == '\0')
		return (NULL);
	while (buffer[len] && buffer[len] != '\n')
		len++;
	line = malloc(len + 2);
	if (!line)
		return (NULL);
	len = 0;
	while (buffer[len] && buffer[len] != '\n')
	{
		line[len] = buffer[len];
		len++;
	}
	if (buffer[len] == '\n')
		line[len++] = '\n';
	line[len] = '\0';
	return (line);
}

static char	*update_buffer(char *buffer)
{
	size_t	len;
	size_t	i;
	char	*new_buffer;

	len = 0;
	while (buffer[len] && buffer[len] != '\n')
		len++;
	if (!buffer[len])
	{
		free(buffer);
		return (NULL);
	}
	new_buffer = malloc(gnl_strlen(buffer) - len + 1);
	if (!new_buffer)
	{
		free(buffer);
		return (NULL);
	}
	len++;
	i = 0;
	while (buffer[len])
		new_buffer[i++] = buffer[len++];
	new_buffer[i] = '\0';
	free(buffer);
	return (new_buffer);
}

static int	read_and_append(int fd, char **buffer)
{
	char	*tmp;
	int		bytes_read;

	tmp = malloc(BUFFER_SIZE + 1);
	if (!tmp)
		return (MALLOC_ERROR);
	bytes_read = 1;
	while (!gnl_strchr(*buffer, '\n') && bytes_read > 0)
	{
		bytes_read = read(fd, tmp, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			free(tmp);
			return (READ_ERROR);
		}
		tmp[bytes_read] = '\0';
		*buffer = gnl_strjoin(*buffer, tmp);
		if (!*buffer)
		{
			free(tmp);
			return (MALLOC_ERROR);
		}
	}
	free(tmp);
	return (READ_SUCCESS);
}

char	*get_next_line(int fd)
{
	static char	*buffer = NULL;
	char		*line;

	if (BUFFER_SIZE <= 0 || fd <= 0)
		return (NULL);
	if (read_and_append(fd, &buffer) == MALLOC_ERROR)
		return (NULL);
	line = extract_line(buffer);
	buffer = update_buffer(buffer);
	return (line);
}

// #include <fcntl.h>
// #include <stdio.h>
// #include "get_next_line.h"

// int main(void)
// {
// 	int fd;
// 	char *line;

// 	fd = open("test.txt", O_RDONLY);
// 	if (fd < 0)
// 	{
// 		perror("Error opening file");
// 		return(1);
// 	}
// 	while((line = get_next_line(fd)) != NULL)
// 	{
// 		printf("%s", line);
// 		free(line);
// 	}
// 	close(fd);
// 	return(0);
// }
