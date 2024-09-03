/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aneumann <aneumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 12:34:26 by aneumann          #+#    #+#             */
/*   Updated: 2024/09/03 16:01:26 by aneumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_pids(t_data *data)
{
	t_cmd	*current;
	
	current = data->cmd;
	while (current != NULL)
	{
		printf("PIPE IN = %d\n", current->pipe_in);
		printf("PIPE OUT = %d\n", current->pipe_out);
		current = current->next;
	}
}

int	executor(t_data *data)
{
	if (is_simple_command(data->cmd))
		return (data->cmd->built_in(data, data->cmd), 1);
	if (!ft_create_pipes(data))
		return (free_data(data, EXIT_FAILURE), 0);
	print_pids(data);
//ROB faut ouvrir les redirections pour pipe_in de la premiere commande ou autre
	if (!ft_open_redir(data))
		return (free_data(data, EXIT_FAILURE), 0);
	if (!ft_execute_cmd(data->cmd))
	 	return (free_data(data, EXIT_FAILURE), 0);
	printf("4 OK\n");
	set_exit_code(data, 0);
	printf("5 OK\n");
	return (1);

}

//ROB probablement comme ca
//faut voir
int	ft_create_pipes(t_data *data)
{
	int		pipes[2];
	t_cmd	*current;

	current = data->cmd;
	ft_memset(pipes, 0, sizeof(int));
	while (current != NULL)
	{
		if (current->next != NULL)
		{
			if (pipe(pipes) == -1)
			{
				perror("pipe");
				return (0);
			}
			current->pipe_out = pipes[0];
			current->next->pipe_in = pipes[1];
			ft_memset(pipes, 0, sizeof(int));
		}
		else
			current->pipe_out = 1;
		current = current->next;
	}
	return (1);
}

// bool	ft_execute(t_data *data)
// {
// 	t_cmd	*current;

// 	current = data->cmd;
// 	printf("1.1 OK\n");
// 	while (current != NULL)
// 	{
// 		if (!ft_execute_cmd(current))
// 			return (false);
// 		printf("1.2 OK\n");
// 		current = current->next;
// 	}
// 	return (true);
// }

bool 	ft_execute_cmd(t_cmd *cmd)
{	
	int pid;
	int i;
	t_cmd *current;
	
	current = cmd;
	if (!allocate_pids(cmd->data_p))
		return (false);
	i = 0;
	while (i < current->data_p->size)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return (false);
		}
		printf("2.3/7 OK apres fork\n");
		if ( pid == 0)
		{
			printf("2.4/7 childdd OK\n");
			
			child(current, current->pipe_in, current->pipe_out); // doute sur pipe_in / pipe_out
		}
		else if (pid > 0)
		{
			current->data_p->child_pids[i] = pid;
			close(current->pipe_out);
			close(current->pipe_in);
			wait(NULL);
		}
		else
			return (ft_close_all_fds(cmd->redirection), false);
		current = current->next;
		i++;
	}
	printf("2.5/7 OK\n");
	ft_close_all_fds(cmd->redirection);
	printf("2.6/7 OK\n");
	ft_waitpid(cmd->data_p);
	printf("2.7/7 OK\n");
	return (true);
}

//fonction waitpid pour attendre la fin de chaque processus enfant
bool    ft_waitpid(t_data *data)
{
	int i;
	int status;

	i = 0;
	printf("waitpid 1/4 ok\n");
	while (i < data->size)
	{
		waitpid(data->child_pids[i], &status, 0);
		printf("waitpid 2/4 ok\n");
		i++;
	}
	printf("waitpid 3/4 ok\n");
	handle_exitcode(data, i, status);
	printf("waitpid 4/4 fin de la fonction waitpid\n");
	return (true);
}

void child(t_cmd *cmd, int input, int output)
{
	char	**tab;

	tab = var_list_to_tab(cmd->data_p->var);
	printf("child 1/7 ok\n");
	ft_redirect(cmd, input, output);
	printf("child 2/7 ok\n");
	//ft_free_pipes(cmd);
	if (cmd->built_in)
	{
		cmd->built_in(cmd->data_p, cmd);
	}
	// printf("child 3/7 ok\n");
	else if (execve(cmd->args[0], cmd->args, tab) == -1)
	{
		perror("execve");
		free_mem(tab);
		free_data(cmd->data_p, EXIT_FAILURE);
	}
	printf("CHILD OK\n");
	//free_data(cmd->data_p, EXIT_SUCCESS);
}

//fonction dup2 pour rediriger l'entree et la sortie standard
void	ft_redirect(t_cmd *cmd, int input, int output)
{
	printf("input = %d\n", input);
	printf("output = %d\n", output);
	if(dup2(input, STDIN_FILENO) == -1)
	{
		perror("dup2");
		free_data(cmd->data_p, EXIT_FAILURE);
	}
	if ( input != STDIN_FILENO)
		close(input);
	printf("REDIRECT 3 OK\n"); ////////////////////////////////////////////////////////////////
	// if (dup2(output, STDOUT_FILENO) == -1)
	if (dup2(output, cmd->next->pipe_in) == -1)
	{
		perror("dup2");
		free_data(cmd->data_p, EXIT_FAILURE);
	}
	printf("REDIRECT 4 OK\n");
	if (output != STDOUT_FILENO)
		close(output);
	printf("REDIRECT 5 OK\n");
}