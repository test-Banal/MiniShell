/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 13:17:44 by roarslan          #+#    #+#             */
/*   Updated: 2024/09/23 17:59:23 by roarslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_home(t_var *var)
{
	t_var	*current;

	current = var;
	while (current != NULL)
	{
		if (ft_strcmp("HOME", current->name) == 0)
			return (ft_strdup(current->value));
		current = current->next;
	}
	return (ft_strdup(""));
}

char	*get_oldpwd(t_var *var)
{
	t_var	*current;

	current = var;
	while (current != NULL)
	{
		if (ft_strcmp("PWD", current->name) == 0)
			return (ft_strdup(current->value));
		current = current->next;
	}
	return (ft_strdup(""));
}

int	cd_minus(t_data *data)
{
	char	*tmp;
	char	*current_pwd;

	tmp = ft_strdup(data->old_pwd);
	if (chdir(tmp) != 0)
	{
		perror("cd");
		free(tmp);
		return (set_exit_code(data, 2), 1);
	}
	current_pwd = ft_strdup(data->pwd);
	free(data->pwd);
	data->pwd = ft_strdup(tmp);
	free(tmp);
	free(data->old_pwd);
	data->old_pwd = ft_strdup(current_pwd);
	set_pwd(data, data->pwd, data->old_pwd);
	free(current_pwd);
	return (0);
}

int	cd_home(t_data *data)
{
	char	*home;
	char	*current_pwd;

	home = get_home(data->var);
	if (chdir(home) != 0)
	{
		perror("cd");
		return (set_exit_code(data, 1), 1);
	}
	current_pwd = ft_strdup(home);
	free(home);
	free(data->old_pwd);
	data->old_pwd = ft_strdup(data->pwd);
	free(data->pwd);
	data->pwd = ft_strdup(current_pwd);
	free(current_pwd);
	set_pwd(data, data->pwd, data->old_pwd);
	return (0);
}

int	cd_helper(t_data *data, t_cmd *cmd)
{
	char	*tmp;

	tmp = get_oldpwd(data->var);
	if (chdir(cmd->args[1]) == -1)
	{
		perror("cd");
		set_exit_code(data, 1);
	}
	free(data->old_pwd);
	data->old_pwd = ft_strdup(tmp);
	free(data->pwd);
	data->pwd = getcwd(NULL, 0);
	if (!data->pwd)
	{
		set_exit_code(data, 1);
		return (free(tmp), 1);
	}
	set_pwd(data, data->pwd, data->old_pwd);
	free(tmp);
	return (0);
}
