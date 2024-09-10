/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 13:21:51 by roarslan          #+#    #+#             */
/*   Updated: 2024/09/03 16:54:56 by roarslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**var_list_to_tab(t_var *var)
{
	int		i;
	int		len;
	char	**dest;
	t_var	*current;

	len = ft_var_lstsize(var);
	dest = malloc(sizeof(char *) * (len + 1));
	if (!dest)
		return (NULL);
	i = 0;
	current = var;
	while (current)
	{
		len = ft_strlen(current->name) + ft_strlen(current->value) + 2;
		dest[i] = malloc(sizeof(char) * len);
		if (!dest[i])
			return (free_mem(dest), NULL);
		dest[i] = ft_strcpy(dest[i], current->name);
		dest[i] = ft_strcpy(dest[i], "=");
		dest[i] = ft_strcpy(dest[i], current->value);
		current = current->next;
		i++;
	}
	dest[i] = NULL;
	return (dest);
}

//set_exit_code(cmd->data_p, exit_code);
void	set_exit_code(t_data *data, int exit_code)
{
	t_var	*current;

	current = data->var;
	data->exit_code = exit_code;
	while (current != NULL)
	{
		if (ft_strcmp(current->name, "?") == 0)
		{
			free(current->value);
			current->value = ft_itoa(exit_code);
		}
		current = current->next;
	}
}

void	get_pwd(t_data *data)
{
	t_var	*current;
	
	current = data->var;
	while (current != NULL)
	{
		if (ft_strcmp(current->name, "PWD") == 0)
			data->pwd = ft_strdup(current->value);
		current = current->next;
	}
	current = data->var;
	while (current != NULL)
	{
		if (ft_strcmp(current->name, "OLDPWD") == 0)
			data->old_pwd = ft_strdup(current->value);
		current = current->next;
	}
}

void	set_pwd2(t_data *data, char *pwd, char *oldpwd)
{
	if (data->pwd)
		free(data->pwd);
	data->pwd = ft_strdup(pwd);
	if (data->old_pwd)
		free(data->old_pwd);
	data->old_pwd = ft_strdup(oldpwd);
}

void	set_pwd(t_data *data, char *pwd, char *oldpwd)
{
	t_var	*current;

	current = data->var;
	while (current != NULL)
	{
		if (ft_strcmp(current->name, "PWD") == 0)
		{
			free(current->value);
			current->value = ft_strdup(pwd);
			break ;
		}
		current = current->next;
	}
	current = data->var;
	while (current != NULL)
	{
		if (ft_strcmp(current->name, "OLDPWD") == 0)
		{
			free(current->value);
			current->value = ft_strdup(oldpwd);
			break ;
		}
		current = current->next;
	}
	set_pwd2(data, pwd, oldpwd);
}
