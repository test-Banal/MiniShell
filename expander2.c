/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 18:58:13 by roarslan          #+#    #+#             */
/*   Updated: 2024/09/20 15:40:01 by roarslan         ###   ########.fr       */
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
			return (free(var_name), ft_strdup(current->value));
		}
		current = current->next;
	}
	free(var_name);
	return (ft_strdup(""));
}

void	toggle_quotes(t_data *data, char c)
{
	if (c == '\'' && data->in_double == 0)
		data->in_single = !data->in_single;
	else if (c == '\"' && data->in_single == 0)
		data->in_double = !data->in_double;
}

void	check_for_var2(t_data *data, char **dest, int *i, char *str)
{
	char	*var_value;
	int		end;
	char	*var_name;
	char	*tmp;

	data->in_single = 0;
	data->in_double = 0;
	while (str[*i])
	{
		toggle_quotes(data, str[*i]);
		if (str[*i] == '$' && data->in_single == 0)
		{
			tmp = ft_substr(str, data->start, *i - data->start);
			*dest = ft_strjoin3(*dest, tmp);
			end = find_end_var(str, *i);
			var_name = ft_substr(str, *i + 1, end - *i - 1);
			var_value = change_var(data, var_name);
			*dest = ft_strjoin3(*dest, var_value);
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
	char	*sign;

	i = 0;
	sign = NULL;
	if (str[i] == '$' && ft_strlen(str) == 1)
		sign = ft_strdup("$");
	data->start = 0;
	dest = ft_strdup("");
	check_for_var2(data, &dest, &i, str);
	if (data->start < i)
	{
		tmp = ft_substr(str, data->start, i - data->start);
		dest = ft_strjoin3(dest, tmp);
	}
	free(current->args[j]);
	if (sign != NULL)
	{
		current->args[j] = ft_strdup(sign);
		free(sign);
	}
	else
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
			if (ft_strchr(current->args[i], '$'))
			{
				len = ft_strlen(current->args[i]);
				tmp = ft_substr(current->args[i], 0, len);
				check_for_var(data, tmp, current, i);
				free(tmp);
			}
			i++;
		}
		current = current->next;
	}
}
