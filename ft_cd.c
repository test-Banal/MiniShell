/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 13:17:44 by roarslan          #+#    #+#             */
/*   Updated: 2024/09/20 12:35:36 by roarslan         ###   ########.fr       */
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
	free(*pwd);
	free(*oldpwd);
}

void	cd_home(t_data *data, char **pwd, char **oldpwd)
{
	char	*home;

	*oldpwd = getcwd(NULL, 0);
	home = get_home(data->var);
	chdir(home);
	*pwd = getcwd(NULL, 0);
	set_pwd(data, *pwd, *oldpwd);
	free(*pwd);
	free(*oldpwd);
}

void	cd_helper(t_data *data, t_cmd *cmd)
{
	char	*pwd;
	char	*oldpwd;

	oldpwd = getcwd(NULL, 0);
	if (chdir(cmd->args[1]) == -1)
	{
		perror("cd");
		set_exit_code(data, 1);
	}
	pwd = getcwd(NULL, 0);
	set_pwd(data, pwd, oldpwd);
	free(pwd);
	free(oldpwd);
}
