/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 11:59:37 by roarslan          #+#    #+#             */
/*   Updated: 2024/08/26 11:41:09 by roarslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_name(char *str)
{
	char	*dest;
	int		i;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	dest = malloc(sizeof(char) * i + 1);
	if (!dest)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '=')
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
	while (str[i] && str[i] != '=')
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
	var->name = name;
	var->value = value;
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

// void    printvar(t_data *data)
// {
//     t_var    *current;

//     current = data->var;
//     while (current != NULL)
//     {
//         printf("id = [%d]\nname = %s\nvalue = %s\n", current->id, current->name, current->value);
//         current = current->next;
//     }
// }

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
		i++;
	}
	return (1);
}

char	**var_list_to_tab(t_var *var)
{
	int		i;
	int		len;
	char	**dest;
	t_var	*current;

	len = ft_lstsize(var);
	dest = malloc(sizeof(char *) * len + 1);
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
		ft_strcat(dest[i], current->name);
		ft_strcat(dest[i], "=");
		ft_strcat(dest[i], current->value);
		current = current->next;
		i++;
	}
	dest[i] = NULL;
	return (dest);
}

char	*is_var(char *str, int i, t_data *data, int end)
{
	t_var	*current;

	current = data->var;
	if (str[i] == '$'
		&& (str[i + 1] == ' ' || str[i + 1] == '"'
			|| str[i + 1] == '\0'))
		return ("$");
	while (current != NULL)
	{
		if (ft_strncmp(current->name, str + i + 1, end) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

int	var_lenght(char *str, t_data *data)
{
	t_var	*current;

	current = data->var;
	while (current != NULL)
	{
		if (ft_strcmp(current->value, str) == 0)
			return (ft_strlen(current->name) + 1);
		current = current->next;
	}
	return (0);
}
