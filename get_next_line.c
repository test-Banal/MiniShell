/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 12:19:44 by aneumann          #+#    #+#             */
/*   Updated: 2024/09/20 12:37:03 by roarslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_sub(char **rest, char **line)
{
	char	*str;

	str = NULL;
	if (*rest)
	{
		*line = *rest;
		str = ft_strchr(*rest, '\n');
		if (str)
		{
			str++;
			if (*str != '\0')
				*rest = ft_strdup_gnl(str);
			else
				*rest = NULL;
			*str = '\0';
		}
		else
			*rest = NULL;
	}
	else
	{
		*line = (char *)malloc(sizeof(char) * 1);
		*line[0] = '\0';
	}
	return (str);
}

static char	*ft_sub_2(char **rest, char **line, char **buf)
{
	char	*str;
	char	*tmp;

	str = ft_strchr(*buf, '\n');
	if (str)
	{
		str++;
		if (*str != '\0')
			*rest = ft_strdup_gnl(str);
		*str = '\0';
	}
	tmp = *line;
	*line = ft_strjoin_gnl(*line, *buf);
	free(tmp);
	return (str);
}

char	*get_next_line(int fd)
{
	char		*buf;
	int			i;
	char		*str;
	static char	*rest[1024];
	char		*line;

	if (BUFFER_SIZE < 1 || read(fd, 0, 0) == -1 || fd < 0)
		return (NULL);
	buf = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buf)
		return (NULL);
	str = ft_sub(&rest[fd], &line);
	i = 1;
	while (!str && i)
	{
		i = read(fd, buf, BUFFER_SIZE);
		buf[i] = '\0';
		str = ft_sub_2(&rest[fd], &line, &buf);
	}
	free(buf);
	if (ft_strlen(line) > 0)
		return (line);
	free (line);
	return (NULL);
}
/*
int main(void)
{
	int 			fd;
	char 		*str;

	fd = open("get_next_line.c", O_RDONLY);
	while ((str = get_next_line(fd)))
	{
	
		printf("%s", str);
		free(str);
	}
	close(fd);
	free(str);
}
*/
