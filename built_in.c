/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aneumann <aneumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 15:24:40 by roarslan          #+#    #+#             */
/*   Updated: 2024/08/29 14:49:02 by aneumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//oeoeoeoe

void	cd_function(t_data *data, t_cmd *cmd)
{
	(void)data;
	printf("cd ta mere\n");
}

void	echo_function(t_data *data, t_cmd *cmd)
{
	(void)data;
	printf("echo ta mere\n");
}

void	env_function(t_data *data, t_cmd *cmd)
{
	(void)data;
	printf("env ta mere\n");
}

void	exit_function(t_data *data, t_cmd *cmd)
{
	(void)data;
	printf("exit ta mere\n");
}

void	export_function(t_data *data, t_cmd *cmd)
{
	(void)data;
	printf("export ta mere\n");
}


//bash : env - i : affiche correctmenent le chemin
void	pwd_function(t_data *data, t_cmd *cmd)
{
	(void)data;
	printf("pwd ta mere\n");
}

void	unset_function(t_data *data, t_cmd *cmd)
{
	(void)data;
	printf("unset ta mere\n");
}

//oeoeoeoe
bool	builtin_selection(t_data *data, int i)
{
	t_cmd	*cmd;

	cmd = data->cmd + i;
	if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
		echo_command(data, cmd);
	else if (ft_strncmp(data->cmd[i].args[0], "exit", 5) == 0)
		exit_command(data, cmd);
	else if (ft_strncmp(cmd->args[0], "env", 4) == 0)
		env_command(data, cmd);
	else if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
		pwd_command(data, cmd);
	else if (ft_strncmp(cmd->args[0], "export", 7) == 0)
		export_command(data, cmd);
	else if (ft_strncmp(cmd->args[0], "unset", 6) == 0)
		unset_command(data, cmd);
	else if (ft_strncmp(cmd->args[0], "cd", 3) == 0)
		cd_command(data, cmd);
	return (true);
}