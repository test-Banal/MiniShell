/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 18:58:13 by roarslan          #+#    #+#             */
/*   Updated: 2024/08/26 18:59:33 by roarslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*change_var(t_data *data, char *var_name)
{
	t_var	*current;

	current = data->var;
	while (current != NULL)
	{
		if (ft_strcmp(current->name, var_name) == 0)
		{
			return (ft_strdup(current->value));
		}
		current = current->next;
	}
	return (ft_strdup(""));
}

void	check_for_var2(t_data *data, char **dest, int *i, char *str)
{
	char	*var_value;
	int		end;
	char	*var_name;
	char	*tmp;

	while (str[*i])
	{
		if (str[*i] == '$')
		{
			tmp = ft_substr(str, data->start, *i - data->start);
			*dest = ft_strjoin_free(*dest, tmp);
			end = find_end_var(str, *i);
			var_name = ft_substr(str, *i + 1, end - *i - 1);
			var_value = change_var(data, var_name);
			free(var_name);
			*dest = ft_strjoin_free(*dest, var_value);
			// free(var_value); //double free ???
			*i = end;
			data->start = *i;
		}
		else
			*(i) += 1;
	}
}

void	check_for_var(t_data *data, char *str, t_cmd *current, int j)
{
	int		i;
	char	*tmp;
	char	*dest;

	i = 0;
	data->start = 0;
	dest = ft_strdup("");
	check_for_var2(data, &dest, &i, str);
	if (data->start < i)
	{
		tmp = ft_substr(str, data->start, i - data->start);
		dest = ft_strjoin_free(dest, tmp);
		free(tmp);
	}
	free(current->args[j]);
	current->args[j] = ft_strdup(dest);
	free(dest);
}

void	expand_var_quotes(t_data *data)
{
	t_cmd	*current;
	char	*tmp;
	int		i;
	int		len;

	current = data->cmd;
	while (current != NULL)
	{
		i = 0;
		while (current->args[i])
		{
			if (current->args[i][0] == '\"' && ft_strchr(current->args[i], '$'))
			{
				len = ft_strlen(current->args[i]) - 2;
				tmp = ft_substr(current->args[i], 1, len);
				check_for_var(data, tmp, current, i);
				free(tmp);
			}
			i++;
		}
		current = current->next;
	}
}

void	expand_var(t_data *data)
{
	t_cmd	*current;
	int		i;
	char	*tmp;

	current = data->cmd;
	while (current != NULL)
	{
		i = 0;
		while (current->args[i])
		{
			if (current->args[i][0] == '$')
			{
				tmp = change_var(data, current->args[i] + 1);
				free(current->args[i]);
				current->args[i] = ft_strdup(tmp);
				free(tmp);
			}
			i++;
		}
		current = current->next;
	}
}
