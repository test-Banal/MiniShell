/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 19:04:34 by roarslan          #+#    #+#             */
/*   Updated: 2024/09/19 15:17:25 by roarslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_redir_list(t_redir *redir)
{
	t_redir	*current;
	t_redir	*next;

	current = redir;
	while (current != NULL)
	{
		next = current->next;
		if (current->str != NULL)
			free(current->str);
		if (current->path != NULL)
			free(current->path);
		free(current);
		current = next;
	}
}

void	free_cmd_list(t_data *data)
{
	t_cmd	*current;
	t_cmd	*next;

	current = data->cmd;
	while (current != NULL)
	{
		next = current->next;
		if (current->args != NULL)
			free_mem(current->args);
		if (current->redirection != NULL)
			free_redir_list(current->redirection);
		if (current->heredoc != NULL)
			free(current->heredoc);
		free(current);
		current = next;
	}
	data->cmd = NULL;
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*dest;
	size_t	i;
	size_t	slen;

	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		return (ft_strdup(""));
	slen = ft_strlen(s + start);
	if (len > slen)
		len = slen;
	dest = malloc(sizeof(char) * (len + 1));
	if (!dest)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dest[i] = s[start + i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strdup(const char *s)
{
	char	*d;
	int		i;

	d = malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!d)
		return (NULL);
	i = 0;
	while (s[i] != '\0')
	{
		d[i] = s[i];
		i++;
	}
	d[i] = '\0';
	return (d);
}

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}
