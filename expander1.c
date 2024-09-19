/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 15:10:13 by roarslan          #+#    #+#             */
/*   Updated: 2024/09/18 21:20:44 by roarslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_end_var(char *str, int i)
{
	i++;
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
	bins = get_binarys(data);
	if (!bins)
		return (NULL);
	cmd_path = find_command_path(str, bins);
	free_mem(bins);
	if (!cmd_path)
		return (ft_strdup(str));
	return (cmd_path);
}

void	delete_quotes(t_data *data)
{
	t_cmd	*current;
	int		i;
	char	*tmp;
	char	*new_str;
	int		len;

	current = data->cmd;
	while (current != NULL)
	{
		i = 0;
		while (current->args[i])
		{
			tmp = current->args[i];
			len = ft_strlen(tmp);
			if ((tmp[0] == '\'' && tmp[len - 1] == '\'')
				|| (tmp[0] == '\"' && tmp[len - 1] == '\"'))
				new_str = ft_substr(tmp, 1, len - 2);
			else
				new_str = ft_strdup(tmp);
			free(current->args[i]);
			current->args[i] = new_str;
			i++;
		}
		current = current->next;
	}
}

// void	delete_quotes(t_data *data)
// {
// 	t_cmd	*current;
// 	int		i;
// 	char	*tmp;
// 	int		len;

// 	current = data->cmd;
// 	while (current != NULL)
// 	{
// 		i = 0;
// 		while (current->args[i])
// 		{
// 			if (current->args[i][0] == '\'' || current->args[i][0] == '\"')
// 			{
// 				len = ft_strlen(current->args[i]) - 2;
// 				tmp = ft_substr(current->args[i], 1, len);
// 				free(current->args[i]);
// 				current->args[i] = ft_strdup(tmp);
// 				free(tmp);
// 			}
// 			i++;
// 		}
// 		current = current->next;
// 	}
// }

void	expander(t_data *data)
{
	char	*tmp;
	t_cmd	*current;

	// expand_var(data);
	expand_var_quotes(data);
	delete_quotes(data);//? a refaire differement si des quotes ne sont pas supprimes avant
	current = data->cmd;
	tmp = NULL;
	while (current != NULL)
	{
		if (current->args[0] && current->args[0][0] != '$')
		{
			tmp = expand_cmd(current->args[0], data);
			if (tmp)
			{
				free(current->args[0]);
				current->args[0] = ft_strdup(tmp);
				free(tmp);
			}
		}
		current = current->next;
	}
}
