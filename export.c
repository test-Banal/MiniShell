/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 15:45:43 by roarslan          #+#    #+#             */
/*   Updated: 2024/09/24 07:41:36 by roarslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_value_export(char *str)
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
	if (str[i] == '=' || str[i] == '+')
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

void	update_var(t_var *var, char *new_value)
{
	free(var->value);
	var->value = ft_strdup(new_value);
	free(new_value);
}

void	export_var_with_value(char **args, t_data *data, int *i)
{
	char	*name;
	char	*value;
	t_var	*var;

	name = get_name(args[*i]);
	value = get_value(args[*i]);
	var = data->var;
	while (var != NULL && ft_strcmp(name, "?") != 0)
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
	if (!name[0])
		return ;
	while (var != NULL)
	{
		if (ft_strcmp(var->name, name) == 0)
		{
			free(name);
			(*i)++;
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
		if (ft_strcmp(current->name, "?") == 0)
			current = current->next;
		else if (current)
		{
			printf("%s", current->name);
			if (ft_strcmp(current->value, "") == 0)
				printf("=\"\"");
			else
				printf("=\"%s\"", current->value);
			printf("\n");
			current = current->next;
		}
	}
}
