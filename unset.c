/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 18:40:13 by roarslan          #+#    #+#             */
/*   Updated: 2024/09/20 12:22:08 by roarslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	unset_var(t_data *data, char *str)
{
	t_var	*current;
	t_var	*previous;

	current = data->var;
	previous = NULL;
	while (current != NULL)
	{
		if (ft_strcmp(str, current->name) == 0)
		{
			if (previous == NULL)
				data->var = current->next;
			else
				previous->next = current->next;
			free(current->name);
			free(current->value);
			free(current);
			return ;
		}
		previous = current;
		current = current->next;
	}
}

void	unset_function(t_data *data, t_cmd *cmd)
{
	int		i;

	i = 1;
	read_pipe_in(cmd);
	while (cmd->args[i] != NULL)
	{
		unset_var(data, cmd->args[i]);
		i++;
	}
}
