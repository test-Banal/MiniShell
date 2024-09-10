/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 12:34:13 by aneumann          #+#    #+#             */
/*   Updated: 2024/09/10 15:23:12 by roarslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_simple_command(t_cmd *cmd)
{
	cmd->data_p->size = ft_cmd_lstsize(cmd);
	if (cmd->data_p->size != 1)
		return (0);
	if (ft_strcmp(cmd->args[0], "export") == 0
		|| ft_strcmp(cmd->args[0], "unset") == 0
		|| ft_strcmp(cmd->args[0], "exit") == 0
		|| ft_strcmp(cmd->args[0], "cd") == 0)
		return (1);
	return (0);
}

//void	simple_child(t_cmd *cmd)
// {
// 	char	*str;
// 	char	**tab;
	
// 	str = ft_strdup(cmd->args[0]);
// 	tab = var_list_to_tab(cmd->data_p->var);
// 	// if (cmd->redirection)
// 	// 	ft_redirect(cmd, cmd->pipe_in, cmd->pipe_out);
// 	if (cmd->pipe_in != 0)
// 		dup2(cmd->pipe_in, 0);
// 	if (cmd->pipe_out != 1)
// 		dup2(cmd->pipe_out, 1);
// 	if(execve(str, cmd->args, tab) == -1);
// 	{	perror("execve");
// 		exit(EXIT_FAILURE);
// 	}
// }


//alocate pid : creer un tableau de pid_t de la taille du nombre de commandes
// et initialise chaque case a -1, pourque le processus parent verifier 
//que le processus enfant a bien ete execute. 
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

// redir->id n'est pas un fd
//cette fontion ne ferme pas les fd!
// a refaire ouaip
bool	ft_close_all_fds(t_redir *redir)
{
	t_redir	*current;

	current = redir;
	while (current)
	{
		if (current->id != -1)
			close(current->id);
		current = current->next;
	}
	return (true);
}

//exitcode
void	handle_exitcode(t_data *data, int i, int status)
{
	//printf("handle_exitcode 1 ok\n");
	(void)i; // erreur de Makefile
	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == 0)
			set_exit_code(data, 0);
		else
			set_exit_code(data, WEXITSTATUS(status));
	//	printf("handle_exitcode 2 ok\n");
	}
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == 2)
			set_exit_code(data, 130);
		else if (WTERMSIG(status) == 3)
			set_exit_code(data, 131);
		else
			set_exit_code(data, 128 + WTERMSIG(status));
	//	printf("handle_exitcode 3 ok\n");
	}
	else
		set_exit_code(data, 1);
}

