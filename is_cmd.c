/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_cmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 10:08:02 by roarslan          #+#    #+#             */
/*   Updated: 2024/08/27 15:05:47 by roarslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	test_path(char *str, char **bins)
{
	int		i;
	char	*half_path;
	char	*cmd;

	i = 0;
	if ((str[0] == '/') && ft_strchr(str + 1, '/') == NULL)
		return (0);
	while (bins[i])
	{
		half_path = ft_strjoin(bins[i], "/");
		cmd = ft_strjoin(half_path, str);
		if (access(cmd, F_OK | X_OK) == 0)
		{
			free(cmd);
			return (1);
		}
		free(cmd);
		i++;
	}
	return (0);
}

int	test_absolute_path(char *str)
{
	struct stat	sb;

	if ((str[0] == '/') && ft_strchr(str + 1, '/') == NULL)
		return (0);
	if (stat(str, &sb) == 0 && S_ISREG(sb.st_mode) && (sb.st_mode & S_IXUSR))
		return (1);
	return (0);
}

int	is_cmd(char *str, t_data *data)
{
	char	**bins;

	bins = NULL;
	bins = get_binarys(data);
	if (!bins)
		return (0);
	if (test_path(str, bins) || test_absolute_path(str))
	{
		free_mem(bins);
		return (1);
	}
	free_mem(bins);
	return (0);
}

char	*get_path(t_var *var)
{
	t_var	*current;

	current = var;
	while (current != NULL)
	{
		if (ft_strcmp(current->name, "PATH") == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

char	**get_binarys(t_data *data)
{
	char	**dest;
	char	*str;
	t_var	*var;

	dest = NULL;
	var = data->var;
	str = get_path(var);
	if (!str)
		return (NULL);
	dest = ft_split(str, ':');
	if (!dest)
		return (NULL);
	return (dest);
}
