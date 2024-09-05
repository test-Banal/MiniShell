/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 13:17:44 by roarslan          #+#    #+#             */
/*   Updated: 2024/09/04 12:48:58 by roarslan         ###   ########.fr       */
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
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

char	*get_oldpwd(t_var *var)
{
	t_var	*current;

	current = var;
	while (current != NULL)
	{
		if (ft_strcmp("OLDPWD", current->name) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

void	cd_minus(t_data *data, char **pwd, char **oldpwd)
{
	char	*tmp;
	
	*oldpwd = getcwd(NULL, 0);
	tmp = get_oldpwd(data->var);
	chdir(tmp);
	*pwd = getcwd(NULL, 0);
	set_pwd(data, *pwd, *oldpwd);
}

void	cd_home(t_data *data, char **pwd, char **oldpwd)
{
	char	*home;

	*oldpwd = getcwd(NULL, 0);
	home = get_home(data->var);
	chdir(home);
	*pwd = getcwd(NULL, 0);
	set_pwd(data, *pwd, *oldpwd);
}

void	cd_function(t_data *data, t_cmd *cmd)
{
	char	*pwd;
	char	*oldpwd;

	if (tab_size(cmd->args) > 2)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		return ;
	}
	if (tab_size(cmd->args) == 1 || (cmd->args[1] && ft_strcmp(cmd->args[1], "~") == 0))
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
	{
		oldpwd = getcwd(NULL, 0);
		chdir(cmd->args[1]);//gestion d'erreurs?
		pwd = getcwd(NULL, 0);
		set_pwd(data, pwd, oldpwd);
	}
}
