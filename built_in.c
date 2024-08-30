/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 15:24:40 by roarslan          #+#    #+#             */
/*   Updated: 2024/08/26 11:56:06 by roarslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cd_function(t_data *data)
{
	(void)data;
	printf("cd ta mere\n");
}

void	echo_function(t_data *data)
{
	(void)data;
	printf("echo ta mere\n");
}

void	env_function(t_data *data)
{
	(void)data;
	printf("env ta mere\n");
}

void	exit_function(t_data *data)
{
	(void)data;
	printf("exit ta mere\n");
}

void	export_function(t_data *data)
{
	(void)data;
	printf("export ta mere\n");
}

void	pwd_function(t_data *data)
{
	(void)data;
	printf("pwd ta mere\n");
}

void	unset_function(t_data *data)
{
	(void)data;
	printf("unset ta mere\n");
}
