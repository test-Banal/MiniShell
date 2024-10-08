/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 12:34:13 by aneumann          #+#    #+#             */
/*   Updated: 2024/09/21 22:05:11 by roarslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_simple_command(t_cmd *cmd)
{
	cmd->data_p->size = ft_cmd_lstsize(cmd);
	if (cmd->data_p->size != 1)
		return (0);
	if (cmd->args[0] && (ft_strcmp(cmd->args[0], "export") == 0
			|| ft_strcmp(cmd->args[0], "unset") == 0
			|| ft_strcmp(cmd->args[0], "exit") == 0
			|| ft_strcmp(cmd->args[0], "cd") == 0))
		return (1);
	return (0);
}

bool	allocate_pids(t_data *data)
{
	int	i;

	i = 0;
	data->size = ft_cmd_lstsize(data->cmd);
	data->child_pids = malloc(sizeof(int) * data->size + 1);
	if (!data->child_pids)
		return (false);
	while (i < data->size)
	{
		data->child_pids[i] = -1;
		i++;
	}
	return (true);
}

bool	ft_close_all_fds(t_data *data)
{
	t_cmd	*current;

	current = data->cmd;
	while (current != NULL)
	{
		if (current->pipe_in != -1 && !isatty(current->pipe_in))
			close(current->pipe_in);
		if (current->pipe_out != -1 && !isatty(current->pipe_out))
			close(current->pipe_out);
		current = current->next;
	}
	return (true);
}

//exitcode
void	handle_exitcode(t_data *data, int status)
{
	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == 0)
			set_exit_code(data, 0);
		else
			set_exit_code(data, WEXITSTATUS(status));
	}
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == 2)
			set_exit_code(data, 130);
		else if (WTERMSIG(status) == 3)
			set_exit_code(data, 131);
		else
			set_exit_code(data, 128 + WTERMSIG(status));
	}
	else
		set_exit_code(data, 1);
}

void	ft_unlink_heredocs(t_data *data)
{
	t_cmd	*current_cmd;
	t_redir	*current_redir;

	current_cmd = data->cmd;
	while (current_cmd != NULL)
	{
		current_redir = current_cmd->redirection;
		while (current_redir != NULL)
		{
			if (current_redir->id == LESS_LESS)
				unlink(current_redir->next->str);
			current_redir = current_redir->next;
		}
		current_cmd = current_cmd->next;
	}
}
