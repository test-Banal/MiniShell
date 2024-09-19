/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 16:14:39 by roarslan          #+#    #+#             */
/*   Updated: 2024/09/19 11:04:41 by roarslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_nbrlen(long n)
{
	int	len;

	len = 0;
	if (n == 0)
		return (1);
	if (n < 0)
	{
		n = -n;
		len++;
	}
	while (n > 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char	*dest;
	int		i;
	long	nb;

	nb = n;
	dest = malloc(sizeof(char) * (ft_nbrlen(nb) + 1));
	if (!dest)
		return (NULL);
	if (nb == 0)
		dest[0] = '0';
	i = ft_nbrlen(nb);
	dest[i] = '\0';
	if (nb < 0)
	{
		dest[0] = '-';
		nb = -nb;
	}
	while (nb > 0)
	{
		dest[--i] = nb % 10 + '0';
		nb /= 10;
	}
	return (dest);
}

int	ft_cmd_lstsize(t_cmd *cmd)
{
	int		i;
	t_cmd	*current;

	i = 0;
	current = cmd;
	while (current != NULL)
	{
		i++;
		current = current->next;
	}
	return (i);
}

char	*ft_strcat(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*dest;

	i = 0;
	dest = NULL;
	while (s1[i])
	{
		dest[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
		dest[i++] = s2[j++];
	dest[i] = '\0';
	return (dest);
}

int	tab_size(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

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