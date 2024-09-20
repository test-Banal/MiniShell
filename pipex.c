/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aneumann <aneumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 12:34:26 by aneumann          #+#    #+#             */
/*   Updated: 2024/09/20 17:51:36 by aneumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int executor(t_data *data)
{
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

bool ft_execute_pipeline(t_data *data)
{
	t_cmd	*current;
	int		i;

	current = data->cmd;
	i = 0;
	if (!allocate_pids(data))
		return (false);
	ft_heredoc_handler(data);
	if (!syntax_check(data))
	{
		ft_unlink_heredocs(data);
		ft_close_all_fds(data);
		return (ft_putstr_fd("syntax error\n", 2), false);
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
	ft_waitpid(data);
	ft_close_all_fds(data);
	return (true);
}

void	ft_close_fds_child(t_data *data)
{
	int		i;

	i = 0;
	while (i < data->size)
	{
		if (data->pipes[i])
		{
			if (data->pipes[i][0] != -1 && !isatty(data->pipes[i][0]))
				close(data->pipes[i][0]);
			if (data->pipes[i][1] != -1 && !isatty(data->pipes[i][1]))
				close(data->pipes[i][1]);
		}
		i++;
	}
}

void	parent_routine(int index, int pid, t_cmd *cmd)
{
	cmd->data_p->child_pids[index] = pid;
	if (cmd->pipe_out != STDOUT_FILENO && cmd->pipe_out != -1)
		close(cmd->pipe_out);
	if (cmd->pipe_in != STDIN_FILENO && cmd->pipe_in != -1)
		close(cmd->pipe_in);
}

bool ft_execute_cmd(t_cmd *cmd, int index)
{
	pid_t	pid;

    // signal(SIGQUIT, SIG_DFL);// aa tester
    if (!ft_open_redir2(cmd->redirection))
	{
		// perror("redirection failed");
		return (free_data(cmd->data_p, EXIT_FAILURE), false);
	}
	// print_cmd_list(cmd->data_p->cmd); // pour tester
	pid = fork();
	if (pid == -1)
		return (perror("fork"), false);
	if (pid == 0)
	{
		// signal(SIGINT, SIG_DFL); //desactive la gestion des signaux dans le fils
    	// signal(SIGQUIT, SIG_DFL);
		ft_redirect(cmd, cmd->pipe_in, cmd->pipe_out);
		ft_close_fds_child(cmd->data_p);
		child(cmd, cmd->data_p);
	}
	else
		parent_routine(index, pid, cmd);
    return (true);
}

bool ft_waitpid(t_data *data)
{
    int i;
    int status;

    i = 0;
	status = 0;
    while (i < data->size)
	{
        if (waitpid(data->child_pids[i], &status, 0) == -1)
		{
            perror("waitpid");
            return (false);
        }
        handle_exitcode(data, status);
        i++;
    }
    return (true);
}

void	execute_builtin(t_data *data, t_cmd *cmd)
{
	cmd->built_in(data, cmd);
	free_pipex(data);
	free_var_list(data);
	free_cmd_list(data);
	exit(0);
}

void	ft_execve_error(char *str, char **tab, t_data *data)
{
	ft_putstr_fd(str, 2);
	ft_putstr_fd(" command not found\n", 2);
	handle_exitcode(data, 127);
	free(str);
	free_mem(tab);
	free_pipex(data);
	free_var_list(data);
	free_cmd_list(data);
	exit(127);
}

void child(t_cmd *cmd, t_data *data)
{
	char	**tab;
	char	*str;
	
	if (cmd->built_in != NULL)
		execute_builtin(data, cmd);
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
	//global variable for sigquit check
	// if (global_sig[0] == 1)
	// {
	// 	free(str);
	// 	free_mem(tab);
	// 	free_pipex(data);
	// 	free_var_list(data);
	// 	free_cmd_list(data);
	// 	exit(EXIT_FAILURE);
	// } 
	// ft_signal(data, CHILD);
	if (execve(str, cmd->args, tab) == -1)
		ft_execve_error(str, tab, data);
}

void ft_redirect(t_cmd *cmd, int input, int output)
{
    if (input != STDIN_FILENO)
    {
        if (dup2(input, STDIN_FILENO) == -1)
        {
            // perror("dup2 input");
			ft_close_fds_child(cmd->data_p);
			free_pipex(cmd->data_p);
			// set_exit_code(cmd->data_p, EXIT_FAILURE);
			free_var_list(cmd->data_p);
			free_cmd_list(cmd->data_p);
			exit(EXIT_FAILURE);
        }
		if (input != -1)
			close(input);
    }
    if (output != STDOUT_FILENO)
    {
        if (dup2(output, STDOUT_FILENO) == -1)
        {
            // perror("dup2 output");
			ft_close_fds_child(cmd->data_p);
			free_pipex(cmd->data_p);
			// set_exit_code(cmd->data_p, EXIT_FAILURE);
			free_var_list(cmd->data_p);
			free_cmd_list(cmd->data_p);
			exit(EXIT_FAILURE);
        }
		if (output != -1)//test
		close(output);
    }
}
