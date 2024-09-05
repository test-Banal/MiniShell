/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 15:24:40 by roarslan          #+#    #+#             */
/*   Updated: 2024/09/05 16:38:55 by roarslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_function(t_data *data, t_cmd *cmd)
{
	t_var	*current;

	current = data->var;
	while (current != NULL)
	{
		if (current->name != NULL && current->value != NULL && current->value[0] != '\0')
		{
			ft_putstr_fd(current->name, cmd->pipe_out);
			ft_putstr_fd("=", cmd->pipe_out);
			ft_putstr_fd(current->value, cmd->pipe_out);
			ft_putstr_fd("\n", cmd->pipe_out);
		}
		current = current->next;
	}
}

//provisoire?
void	pwd_function(t_data *data, t_cmd *cmd)
{
	t_var	*current;

	current = data->var;
	while (current != NULL)
	{
		if (ft_strcmp(current->name, "PWD") == 0)
		{
			ft_putstr_fd(current->value, cmd->pipe_out);
			ft_putstr_fd("\n", cmd->pipe_out);
			break ;
		}
		current = current->next;
	}
}

int	ft_strstr_1(char *str, char *to_find)
{
	int	i;
	int	j;

	if (!str)
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

void	append_value(t_data *data, char **args, int *i)
{
	t_var	*current;
	char	*name;
	char	*value;
	char	*new_value;

	new_value = NULL;
	current = data->var;
	name = get_name(args[*i]);
	value = get_value(args[*i]);
	printf("NAME = %s\nVALUE = %s\n", name, value);
	while (current != NULL)
	{
		if (ft_strcmp(current->name, name) == 0)
		{
			new_value = ft_strjoin2(current->value, value);
			printf("NEW VALUE = %s\n", new_value);
			update_var(current, new_value);
			free(value);
			free(name);
			(*i)++;
			return ;
		}
		current = current->next;
	}
	free(name);
	free(value);
	export_var_with_value(args, data, i);
}

void	export_function(t_data *data, t_cmd *cmd)
{
	t_var	*current;
	int		i;

	current = data->var;
	i = 1;
	if (tab_size(cmd->args) == 1)
	{
		ft_print_export(data, cmd);
		return ;
	} 
	while (cmd->args[i] != NULL)
	{
		if (ft_strstr_1(cmd->args[i], "+="))
			append_value(data, cmd->args, &i);
		if (ft_strchr(cmd->args[i], '+') && !ft_strchr(cmd->args[i], '='))
			export_error(data, cmd->args[i]);
		if (cmd->args[i] && ft_strchr(cmd->args[i], '='))
			export_var_with_value(cmd->args, data, &i);
		else if (cmd->args[i] && ft_strchr(cmd->args[i], '='))
			export_without_value(cmd->args, data, &i);
	}
}
