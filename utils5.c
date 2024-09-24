/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 16:33:18 by roarslan          #+#    #+#             */
/*   Updated: 2024/09/24 07:38:31 by roarslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strcpy(char *dest, char *src)
{
	int	i;

	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

void	read_pipe_in(t_cmd *cmd)
{
	char	buffer[1024];
	ssize_t	bytes_read;

	bytes_read = 1;
	if (cmd->prev != NULL)
	{
		while (bytes_read > 0)
			bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);
	}
}

char	*ft_strjoin3(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*dest;

	if (!s1 || !s2)
		return (NULL);
	dest = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!dest)
		return (NULL);
	i = 0;
	while (s1[i] != '\0')
	{
		dest[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j] != '\0')
		dest[i++] = s2[j++];
	dest[i] = '\0';
	free(s1);
	free(s2);
	return (dest);
}

int	is_alpha(char c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (1);
	return (0);
}

int	ft_strstr(char *str, char *to_find)
{
	int	i;
	int	j;

	if (!str || !str[0])
		return (0);
	i = 1;
	while (str[i] != '\0')
	{
		j = 0;
		while (to_find[j] == str[i + j] && (i + j) && str[i + j])
			j++;
		if (to_find[j] == '\0')
			return (1);
		i++;
	}
	return (0);
}
