/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 12:34:26 by aneumann          #+#    #+#             */
/*   Updated: 2024/09/10 15:22:49 by roarslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int executor(t_data *data)
{
    if (is_simple_command(data->cmd))
        return (data->cmd->built_in(data, data->cmd), 1);
    if (!ft_create_pipes(data))
	{
        perror("pipe creation failed");
        return (free_data(data, EXIT_FAILURE), 0);
    }
	if (data->size > 1)//a verifier
		ft_assign_pipes(data);///////////////////// 
    if (!ft_open_redir(data)) 
	{
        perror("redirection failed");
        return (free_data(data, EXIT_FAILURE), 0);
    }
	if (!ft_execute_pipeline(data))
    {
        perror("execution failed");
        return (free_data(data, EXIT_FAILURE), 0);
    }
    set_exit_code(data, 0);
   return 1;
}

bool	ft_create_pipes(t_data *data)
{
	int	i;

	i = 0;
    data->size = ft_cmd_lstsize(data->cmd);
	// if (data->size < 2)
	// 	return (true);
	data->pipes = malloc((data->size) + 1 * sizeof(int *));
	if (!data->pipes)
		return (false);
	while (i < data->size)/// size - 1 ? 
	{
		data->pipes[i] = malloc(2 * sizeof(int));
		if (!data->pipes[i])
			return (false);
		if (pipe(data->pipes[i]) == -1)
			return (false);
		i++;
	}
	data->pipes[i] = NULL;
	return (true);
}

bool ft_execute_pipeline(t_data *data)
{
    t_cmd	*current;
	int		i;

    current = data->cmd;
	i = 0;
    if (!allocate_pids(data))
		return false;
    while (current != NULL)
	{
        if (!ft_execute_cmd(current, i))
            return false;
        current = current->next;
		i++;
    }
	ft_waitpid(data);
    return true;
}

bool ft_execute_cmd(t_cmd *cmd, int index)
{
	int		status;
	pid_t	pid;

	status = 0;
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return false;
	}
	if (pid == 0)
	{
		// close(cmd->data_p->pipes[index][0]);
		// close(cmd->data_p->pipes[index][1]);
		child(cmd, cmd->pipe_in, cmd->pipe_out);
	}
	if (pid > 0)
	{
		cmd->data_p->child_pids[index] = pid;
		if (cmd->pipe_out != STDOUT_FILENO)
			close(cmd->pipe_out);
		if (cmd->pipe_in != STDIN_FILENO)
			close(cmd->pipe_in);
		// close(cmd->data_p->pipes[index][1]);
		// close(cmd->data_p->pipes[index][0]);
	}
    // ft_close_all_fds(cmd->redirection);
    // ft_waitpid(cmd->data_p);
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
            return false;
        }
        // handle_exitcode(data, i, status);
        i++;
    }
    return true;
}

void child(t_cmd *cmd, int input, int output)
{
	char	**tab;
	char	*str;

    ft_redirect(cmd, input, output);
    if (cmd->built_in)
	{
        cmd->built_in(cmd->data_p, cmd);
		return ;
    }
	str = ft_strdup(cmd->args[0]);
	tab = var_list_to_tab(cmd->data_p->var);
	if (execve(str, cmd->args, tab) == -1)
	{
		free(str);
		free_mem(tab);
        perror("execve");
        free_data(cmd->data_p, EXIT_FAILURE);
    }
}

void ft_redirect(t_cmd *cmd, int input, int output)
{
    if (input != STDIN_FILENO)
    {
        if (dup2(input, STDIN_FILENO) == -1)
        {
            perror("dup2 input");
            free_data(cmd->data_p, EXIT_FAILURE);
        }
		close(input);
    }
    if (output != STDOUT_FILENO)
    {
        if (dup2(output, STDOUT_FILENO) == -1)
        {
            perror("dup2 output");
            free_data(cmd->data_p, EXIT_FAILURE);
        }
		close(output);
    }
}
