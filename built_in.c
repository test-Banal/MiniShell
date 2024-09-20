/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 15:24:40 by roarslan          #+#    #+#             */
/*   Updated: 2024/09/20 12:35:42 by roarslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_function(t_data *data, t_cmd *cmd)
{
	t_var	*current;

	read_pipe_in(cmd);
	current = data->var;
	while (current != NULL)
	{
		if (current->name != NULL && current->value != NULL \
				&& current->value[0] != '\0')
		{
			printf("%s=%s\n", current->name, current->value);
		}
		current = current->next;
	}
}

void	pwd_function(t_data *data, t_cmd *cmd)
{
	char	*pwd;
	int		i;

	(void)data;
	read_pipe_in(cmd);
	pwd = getcwd(NULL, 0);
	printf("%s\n", pwd);
	free(pwd);
	i = 1;
	if (tab_size(cmd->args) > 1)
	{
		while (cmd->args[i])
		{
			if (cmd->args[i][0] == '-')
				printf("%s ", cmd->args[i]);
			i++;
		}
		printf(" : invalid option(s)\n");
	}
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
	value = get_value_export(args[*i]);
	while (current != NULL)
	{
		if (ft_strcmp(current->name, name) == 0)
		{
			new_value = ft_strjoin2(current->value, value);
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

	read_pipe_in(cmd);
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
		else
			i++;
		if (cmd->args[i] && ft_strchr(cmd->args[i], '+')
			&& !ft_strchr(cmd->args[i], '='))
			export_error(data, cmd->args[i]);
		if (cmd->args[i] && cmd->args[i] && ft_strchr(cmd->args[i], '='))
			export_var_with_value(cmd->args, data, &i);
		else if (cmd->args[i] && !ft_strchr(cmd->args[i], '='))
			export_without_value(cmd->args, data, &i);
	}
}

void	cd_function(t_data *data, t_cmd *cmd)
{
	char	*pwd;
	char	*oldpwd;

	read_pipe_in(cmd);
	if (tab_size(cmd->args) > 2)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		set_exit_code(data, 1);
		return ;
	}
	if (tab_size(cmd->args) == 1
		|| (cmd->args[1] && ft_strcmp(cmd->args[1], "~") == 0))
	{
		cd_home(data, &pwd, &oldpwd);
		return ;
	}
	else if (cmd->args[1] && ft_strcmp(cmd->args[1], "-") == 0)
	{
		cd_minus(data, &pwd, &oldpwd);
		return ;
	}
	else
		cd_helper(data, cmd);
}
