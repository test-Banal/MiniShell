/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 13:21:51 by roarslan          #+#    #+#             */
/*   Updated: 2024/09/03 10:42:51 by roarslan         ###   ########.fr       */
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
	dest = malloc(sizeof(char *) * len + 1);
	if (!dest)
		return (NULL);
	i = 0;
	current = var;
	while (current)
	{
		len = ft_strlen(current->name) + 1;
		// len = ft_strlen(current->name) + ft_strlen(current->value) + 2;
		dest[i] = malloc(sizeof(char) * len);
		if (!dest[i])
			return (free_mem(dest), NULL);
		//peut etre ft_strcat est mieux, faut tester
		dest[i] = ft_strjoin(dest[i], current->name);
		dest[i] = ft_strjoin(dest[i], "=");
		dest[i] = ft_strjoin(dest[i], current->value);
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
