/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 12:13:31 by roarslan          #+#    #+#             */
/*   Updated: 2024/09/03 12:13:59 by roarslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_n_option(char *str)
{
	int	i;

	i = 0;
	if (str[i++] != '-')
		return (0);
	while (str[i] != '\0')
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

void	echo_function(t_data *data, t_cmd *cmd)
{
	int	option_flag;
	int	i;

	i = 1;
	option_flag = 0;
	(void)data;
	if (cmd->args && cmd->args[1] != NULL)
		option_flag = is_n_option(cmd->args[1]);
	if (option_flag == 1)
		i = 2;
	while (cmd->args[i])
	{
		ft_putstr_fd(cmd->args[i], cmd->pipe_out);
		ft_putstr_fd(" ", cmd->pipe_out);
		i++;
	}
	if (option_flag == 0)
		ft_putstr_fd("\n", cmd->pipe_out);
}
