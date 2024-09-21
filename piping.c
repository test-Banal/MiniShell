/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 12:47:43 by roarslan          #+#    #+#             */
/*   Updated: 2024/09/21 14:33:06 by aneumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_create_pipes(t_data *data)
{
	int	i;

	i = 0;
	data->size = ft_cmd_lstsize(data->cmd);
	data->pipes = ft_calloc(data->size, sizeof(int *));
	if (!data->pipes)
		return (false);
	while (i < data->size)
	{
		data->pipes[i] = ft_calloc(2, sizeof(int));
		if (!data->pipes[i])
			return (false);
		if (pipe(data->pipes[i]) == -1)
			return (false);
		i++;
	}
	close(data->pipes[i - 1][0]);
	close(data->pipes[i - 1][1]);
	return (true);
}

void	assign_pipes(t_cmd *cmd, int **pipes, int cmd_index, int total_cmds)
{
	if (cmd_index == 0)
	{
		cmd->pipe_in = STDIN_FILENO;
		cmd->pipe_out = pipes[cmd_index][1];
	}
	else if (cmd_index == total_cmds - 1)
	{
		cmd->pipe_in = pipes[cmd_index - 1][0];
		cmd->pipe_out = STDOUT_FILENO;
	}
	else
	{
		cmd->pipe_in = pipes[cmd_index - 1][0];
		cmd->pipe_out = pipes[cmd_index][1];
	}
}

void	ft_assign_pipes(t_data *data)
{
	t_cmd	*current;
	int		i;

	i = 0;
	current = data->cmd;
	while (current != NULL)
	{
		assign_pipes(current, data->pipes, i, data->size);
		i++;
		current = current->next;
	}
}
