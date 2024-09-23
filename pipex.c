/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 12:34:26 by aneumann          #+#    #+#             */
/*   Updated: 2024/09/23 16:24:54 by roarslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	executor(t_data *data)
{
	if (!syntax_check(data))
		return (ft_putstr_fd("syntax error\n", 2), 0);
	if (is_simple_command(data->cmd))
		return (data->cmd->built_in(data, data->cmd), 1);
	if (!ft_create_pipes(data))
	{
		perror("pipe creation failed");
		return (free_data(data, EXIT_FAILURE), 0);
	}
	if (data->size > 1)
		ft_assign_pipes(data);
	if (!ft_execute_pipeline(data))
		return (free_data(data, EXIT_FAILURE), 0);
	return (1);
}

bool	ft_execute_pipeline(t_data *data)
{
	t_cmd	*current;
	int		i;

	current = data->cmd;
	i = 0;
	if (!allocate_pids(data))
		return (false);
	ft_heredoc_handler(data);
	if (g_sig[1] == 1)
	{
		ft_unlink_heredocs(data);
		ft_close_all_fds(data);
		g_sig[1] = 0;
		return (set_exit_code(data, 130), true);
	}
	while (current != NULL)
	{
		ft_signal(data, CHILD);
		if (!ft_execute_cmd(current, i))
			return (false);
		current = current->next;
		i++;
	}
	ft_unlink_heredocs(data);
	return (ft_waitpid(data), ft_close_all_fds(data), true);
}

void	parent_routine(int index, int pid, t_cmd *cmd)
{
	cmd->data_p->child_pids[index] = pid;
	if (cmd->pipe_out != STDOUT_FILENO && cmd->pipe_out != -1)
		close(cmd->pipe_out);
	if (cmd->pipe_in != STDIN_FILENO && cmd->pipe_in != -1)
		close(cmd->pipe_in);
}

bool	ft_execute_cmd(t_cmd *cmd, int index)
{
	pid_t	pid;

	if (!ft_open_redir2(cmd->redirection))
		return (free_data(cmd->data_p, EXIT_FAILURE), false);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), false);
	if (pid == 0)
	{
		ft_redirect(cmd, cmd->pipe_in, cmd->pipe_out);
		ft_close_fds_child(cmd->data_p);
		child(cmd, cmd->data_p);
	}
	else
		parent_routine(index, pid, cmd);
	return (true);
}

void	child(t_cmd *cmd, t_data *data)
{
	char		**tab;
	char		*str;
	struct stat	statbuf;

	if (cmd->built_in != NULL)
		execute_builtin(data, cmd);
	if (cmd->args[0] && stat(cmd->args[0], &statbuf) == 0
		&& S_ISDIR(statbuf.st_mode))
		ft_execve_dir_error(cmd->args[0], data);
	if (cmd->args[0])
		str = ft_strdup(cmd->args[0]);
	else
	{
		free_pipex(data);
		free_var_list(data);
		free_cmd_list(data);
		exit(1);
	}
	tab = var_list_to_tab(data->var);
	if (execve(str, cmd->args, tab) == -1)
		ft_execve_error(str, tab, data);
}
