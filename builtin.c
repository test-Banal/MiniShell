/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aneumann <aneumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 15:24:40 by roarslan          #+#    #+#             */
/*   Updated: 2024/08/30 12:14:46 by aneumann         ###   ########.fr       */
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

