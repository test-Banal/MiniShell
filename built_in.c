/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 15:24:40 by roarslan          #+#    #+#             */
/*   Updated: 2024/09/03 12:15:27 by roarslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cd_function(t_data *data, t_cmd *cmd)
{
	(void)data;
	(void)cmd;
	printf("cd ta mere\n");
}

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
	char	*pwd;

	(void)data;
	pwd = getcwd(NULL, 0);
	ft_putstr_fd(pwd, cmd->pipe_out);
}

void	export_function(t_data *data, t_cmd *cmd)
{
	t_var	*current;
	int		i;

	current = data->var;
	i = 1;
	if (tab_size(cmd->args) == 1)
		ft_print_export(data, data->cmd);
	while (cmd->args[i] != NULL)
	{
		if (ft_strchr(cmd->args[i], '='))
			export_var_with_value(cmd->args, data, &i);
		else
			export_without_value(cmd->args, data, &i);
		// i++;
	}
}
