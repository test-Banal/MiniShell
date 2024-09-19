/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aneumann <aneumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 12:34:26 by aneumann          #+#    #+#             */
/*   Updated: 2024/09/19 15:15:49 by aneumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//extern int	global_sig[3];
int		g_sig = 0;


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
	if (data->size > 1)
		ft_assign_pipes(data);
    // if (!ft_open_redir(data)) 
	// {
    //     perror("redirection failed");
    //     return (free_data(data, EXIT_FAILURE), 0);
    // }
	// print_cmd_list(data->cmd);
	//heredoc la
	if (!ft_execute_pipeline(data))
    {
        perror("execution failed");
        return (free_data(data, EXIT_FAILURE), 0);
    }
    // set_exit_code(data, 0);
	return (1);
}

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

// bool	ft_create_pipes(t_cmd *cmd, int pipes_fd[2])
// {
// 	t_cmd	*current;
	
// 	current = cmd;
// 	if (pipe(pipes_fd) == -1)
// 	{
// 		perror("pipe");
// 		return (false);
// 	}
// 	current->pipe_out = pipes_fd[1];
// 	current->next->pipe_in = pipes_fd[0];
// 	return (true);
// }

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

//check for heredoc and execute it first, maybe even before piping and all that
bool ft_execute_pipeline(t_data *data)
{
	t_cmd	*current;
	int		i;

	current = data->cmd;
	i = 0;
	if (!allocate_pids(data))
		return (false);
	ft_heredoc_handler(data);
    while (current != NULL)
	{
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
			if (!isatty(data->pipes[i][0]))
				close(data->pipes[i][0]);
			if (!isatty(data->pipes[i][1]))
				close(data->pipes[i][1]);
		}
		i++;
	}
}

bool ft_execute_cmd(t_cmd *cmd, int index)
{
	pid_t	pid;

    // signal(SIGQUIT, SIG_DFL);// aa tester
    if (!ft_open_redir(cmd->data_p))
	{
		perror("redirection failed");
		return (free_data(cmd->data_p, EXIT_FAILURE), 0);
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
		// if (cmd->pipe_out != STDOUT_FILENO)
		// 	close(cmd->pipe_out);
		// if ((cmd->next != NULL && cmd->next->pipe_in != STDIN_FILENO))
		// 	close(cmd->next->pipe_in);
		child(cmd, cmd->data_p);
	}
	if (pid > 0)                 //changer if par else if ALBAN 17/09/2024 a 15h58
	{
		cmd->data_p->child_pids[index] = pid;
		if (cmd->pipe_out != STDOUT_FILENO)
			close(cmd->pipe_out);
		if (cmd->pipe_in != STDIN_FILENO)
			close(cmd->pipe_in);
	}
	// else                                //attention rajout de else et toutes les choses entre {}      ALBAN 17/09/2024 a 15h58
	// {
	// 	printf("ici else dans ft_execute_cmd \n");
	// 	close(cmd->pipe_in);
	// 	close(cmd->pipe_out);
	// }
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

void	sigquit_handler(int signum)
{
	const char	*msg;

	(void)signum;
	msg = "^\\Quit (core dumped)\n";
	write(STDERR_FILENO, msg, ft_strlen(msg));
	//global_sig[0] = 1;
}

void	ft_execve_error(char *str, char **tab, t_data *data)
{
	perror("execve");
	handle_exitcode(data, 127);//?
	free(str);
	free_mem(tab);
	free_pipex(data);
	free_var_list(data);
	free_cmd_list(data);
	exit(EXIT_FAILURE);
}

void child(t_cmd *cmd, t_data *data)
{
	char	**tab;
	char	*str;

	//signal(SIGQUIT, &sigquit_handler);
	// signal(SIGINT, &c_signal);
    // signal(SIGQUIT, SIG_DFL);
	
		
	if (cmd->built_in != NULL)
		execute_builtin(data, cmd);
	if (cmd->args[0])
		str = ft_strdup(cmd->args[0]);
	else
	{
		free_pipex(data);
		free_var_list(data);
		free_cmd_list(data);
		exit(0);
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
    signal(SIGQUIT, SIG_DFL);
	if (execve(str, cmd->args, tab) == -1)
		ft_execve_error(str, tab, data);
}

void	c_signal(int status)
{
	if (status == SIGINT)
	{
		write(STDERR_FILENO, "\n", 1);
		g_sig = 1;
	}
}

void ft_redirect(t_cmd *cmd, int input, int output)
{
    if (input != STDIN_FILENO)
    {
        if (dup2(input, STDIN_FILENO) == -1)
        {
            perror("dup2 input");
			free_pipex(cmd->data_p);
			free_var_list(cmd->data_p);
			free_cmd_list(cmd->data_p);
			set_exit_code(cmd->data_p, EXIT_FAILURE);
			exit(cmd->data_p->exit_code);
        }
		close(input);
    }
    if (output != STDOUT_FILENO)
    {
        if (dup2(output, STDOUT_FILENO) == -1)
        {
            perror("dup2 output");
			free_pipex(cmd->data_p);
			free_var_list(cmd->data_p);
			free_cmd_list(cmd->data_p);
			set_exit_code(cmd->data_p, EXIT_FAILURE); // trouver moyen de changer l'exit code dans le parent
			exit(cmd->data_p->exit_code);
        }
		close(output);
    }
}
