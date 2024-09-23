/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 15:55:01 by aneumann          #+#    #+#             */
/*   Updated: 2024/09/21 21:59:14 by roarslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_redirect(t_cmd *cmd, int input, int output)
{
	if (input != STDIN_FILENO)
	{
		if (dup2(input, STDIN_FILENO) == -1)
			ft_dup2_error(cmd->data_p);
		if (input != -1)
			close(input);
	}
	if (output != STDOUT_FILENO)
	{
		if (dup2(output, STDOUT_FILENO) == -1)
			ft_dup2_error(cmd->data_p);
		if (output != -1)
			close(output);
	}
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

bool	ft_waitpid(t_data *data)
{
	int	i;
	int	status;

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

void	ft_close_fds_child(t_data *data)
{
	int	i;

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
