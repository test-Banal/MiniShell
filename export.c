/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 15:45:43 by roarslan          #+#    #+#             */
/*   Updated: 2024/09/03 12:15:23 by roarslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_var(t_var *var, char *new_value)
{
	free(var->value);
	var->value = ft_strdup(new_value);
	free(new_value);
}

void	export_error(t_data *data, char *value)
{
	ft_putstr_fd("export : ", 2);
	ft_putstr_fd(value, 2);
	ft_putstr_fd("not a valid identifier\n", 2);
	free_data(data, 1);
}

void	export_var_with_value(char **args, t_data *data, int *i)
{
	char	*name;
	char	*value;
	t_var	*var;
	
	name = get_name(args[*i]);
	value = get_value(args[*i]);
	var = data->var;
	while (var != NULL)
	{
		if (name[0] == '\0')
			export_error(data, value);
		if (ft_strcmp(name, var->name) == 0)
		{
			update_var(var, value);
			free(name);
			(*i)++;
			return ;
		}
		var = var->next;
	}
	var = init_env_var(name, value, ft_var_lstsize(data->var) + 1);
	add_var_list(data, var);
	free(name);
	free(value);
	(*i)++;
}

void	export_without_value(char **args, t_data *data, int *i)
{
	char	*name;
	t_var	*var;
	
	var = data->var;
	name = get_name(args[*i]);
	while (var != NULL)
	{
		if (ft_strcmp(var->name, name) == 0)
		{
			free(name);
			return ;
		}
		var = var->next;
	}
	var = init_env_var(name, NULL, ft_var_lstsize(data->var) + 1);
	add_var_list(data, var);
	free(name);
	(*i)++;
}

void	ft_print_export(t_data *data, t_cmd *cmd)
{
	t_var	*current;

	current = data->var;
	(void)cmd;
	while (current != NULL)
	{		
		ft_putstr_fd(current->name, data->cmd->pipe_out);
		if (current->value[0] != '\0')
		{
			ft_putstr_fd("=\"", cmd->pipe_out);
			ft_putstr_fd(current->value, cmd->pipe_out);
			ft_putstr_fd("\"", cmd->pipe_out);
		}
		ft_putstr_fd("\n", cmd->pipe_out);
		current = current->next;
	}
}
