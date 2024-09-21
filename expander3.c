/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 12:28:59 by roarslan          #+#    #+#             */
/*   Updated: 2024/09/21 14:48:17 by aneumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_end_var(char *str, int i)
{
	i++;
	if (!is_alpha(str[i]))
		return (i + 1);
	while (str[i] && ((str[i] >= 'A' && str[i] <= 'Z')
			|| (str[i] >= 'a' && str[i] <= 'z')
			|| (str[i] >= '0' && str[i] <= '9')
			|| str[i] == '_' || str[i] == '?'))
		i++;
	return (i);
}

char	*find_command_path(char *str, char **bins)
{
	char	*half_path;
	char	*cmd;
	int		i;

	if (test_absolute_path(str))
		return (ft_strdup(str));
	i = 0;
	while (bins[i])
	{
		half_path = ft_strjoin2(bins[i], "/");
		cmd = ft_strjoin(half_path, str);
		if (access(cmd, F_OK | X_OK) == 0)
			return (cmd);
		free(cmd);
		i++;
	}
	return (NULL);
}

char	*expand_cmd(char *str, t_data *data)
{
	char	*cmd_path;
	char	**bins;

	bins = NULL;
	if (!str[0])
		return (NULL);
	bins = get_binarys(data);
	if (!bins)
		return (NULL);
	cmd_path = find_command_path(str, bins);
	free_mem(bins);
	if (!cmd_path)
		return (ft_strdup(str));
	return (cmd_path);
}
