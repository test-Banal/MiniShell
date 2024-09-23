/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 13:21:51 by roarslan          #+#    #+#             */
/*   Updated: 2024/09/23 15:24:55 by roarslan         ###   ########.fr       */
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
		dest[i] = ft_strdup(current->name);
		dest[i] = ft_strjoin(dest[i], "=");
		dest[i] = ft_strjoin(dest[i], current->value);
		current = current->next;
		i++;
	}
	dest[i] = NULL;
	return (dest);
}

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
}
