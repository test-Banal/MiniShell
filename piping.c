/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 12:47:43 by roarslan          #+#    #+#             */
/*   Updated: 2024/09/23 13:04:24 by roarslan         ###   ########.fr       */
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

void	assign_pipes(t_cmd *cmd, int **pipes, int index, int total)
{
	if (index == 0)
	{
		cmd->pipe_in = STDIN_FILENO;
		cmd->pipe_out = pipes[index][1];
	}
	else if (index == total - 1)
	{
		cmd->pipe_in = pipes[index - 1][0];
		cmd->pipe_out = STDOUT_FILENO;
	}
	else
	{
		cmd->pipe_in = pipes[index - 1][0];
		cmd->pipe_out = pipes[index][1];
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

void	free_pipex(t_data *data)
{
	int	i;

	i = 0;
	if (data->pipes)
	{
		while (i < ft_cmd_lstsize(data->cmd))
		{
			if (data->pipes[i])
				free(data->pipes[i]);
			i++;
		}
		free(data->pipes);
		data->pipes = NULL;
	}
	if (data->child_pids)
	{
		free(data->child_pids);
		data->child_pids = NULL;
	}
}
