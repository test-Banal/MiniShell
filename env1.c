/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 11:59:37 by roarslan          #+#    #+#             */
/*   Updated: 2024/09/20 12:27:00 by roarslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_name(char *str)
{
	char	*dest;
	int		i;

	i = 0;
	while (str[i] && str[i] != '=' && str[i] != '+')
		i++;
	dest = malloc(sizeof(char) * i + 1);
	if (!dest)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '=' && str[i] != '+')
	{
		dest[i] = str[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*get_value(char *str)
{
	char	*dest;
	int		i;
	int		j;

	i = 0;
	while (str[i] && str[i] != '=' && str[i] != '+')
		i++;
	if (!str[i])
		return (ft_strdup(""));
	i++;
	dest = malloc(sizeof(char) * (ft_strlen(str) - i + 1));
	if (!dest)
		return (NULL);
	j = 0;
	while (str[i])
		dest[j++] = str[i++];
	dest[j] = '\0';
	return (dest);
}

t_var	*init_env_var(char *name, char *value, int i)
{
	t_var	*var;

	var = malloc(sizeof(t_var));
	if (!var)
		return (NULL);
	var->id = i;
	var->name = ft_strdup(name);
	if (value != NULL)
		var->value = ft_strdup(value);
	else
		var->value = ft_strdup("");
	var->next = NULL;
	var->prev = NULL;
	return (var);
}

void	add_var_list(t_data *data, t_var *var)
{
	t_var	*current;

	if (data->var == NULL)
		data->var = var;
	else
	{
		current = data->var;
		while (current->next != NULL)
			current = current->next;
		current->next = var;
		var->prev = current;
	}
}

int	ft_env_to_lst(t_data *data, char **env)
{
	int		i;
	t_var	*var;
	char	*name;
	char	*value;

	i = 0;
	data->var = NULL;
	while (env[i])
	{
		name = get_name(env[i]);
		value = get_value(env[i]);
		if (!name || !value)
			return (free(name), free(value), free_var_list(data), 0);
		var = init_env_var(name, value, i);
		if (!var)
			return (free(name), free(value), free_var_list(data), 0);
		add_var_list(data, var);
		free(name);
		free(value);
		i++;
	}
	name = ft_strdup("?");
	value = ft_itoa(data->exit_code);
	var = init_env_var(name, value, i);
	return (add_var_list(data, var), free(name), free(value), 1);
}
