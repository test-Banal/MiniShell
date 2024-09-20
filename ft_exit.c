/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 10:12:56 by roarslan          #+#    #+#             */
/*   Updated: 2024/09/20 12:20:41 by roarslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//dans exit double free ????
void	ft_free_and_exit(t_data *data, int exit_code)
{
	if (ft_cmd_lstsize(data->cmd) == 1)
		rl_clear_history();
	set_exit_code(data, exit_code);
	free_var_list(data);
	free_pipex(data);
	free_cmd_list(data);
	exit(exit_code);
}

//pas sur d'etre fini
void	exit_function(t_data *data, t_cmd *cmd)
{
	int				i;
	long long int	nb;

	i = 1;
	nb = 0;
	read_pipe_in(cmd);
	while (cmd->args[i] != NULL)
	{
		if (cmd->args[1])
			nb = ft_atolli(cmd->args[1]);
		if (!is_strnum(cmd->args[i]) || nb > LLONG_MAX || nb < LLONG_MIN)
		{
			ft_putstr_fd("exit : numeric argument required\n", 2);
			ft_free_and_exit(data, 2);
		}
		i++;
	}
	if (tab_size(cmd->args) > 2)
	{
		ft_putstr_fd("exit : too many arguments\n", 2);
		set_exit_code(data, 1);
		return ;
	}
	nb = (nb % 256 + 256) % 256;
	ft_free_and_exit(data, nb);
}

long long int	ft_atolli(const char *str)
{
	long long int	res;
	int				sign;
	int				i;

	i = 0;
	res = 0;
	sign = 1;
	while (is_whitespace(str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i])
	{
		res = res * 10 + str[i] - '0';
		i++;
	}
	return (res * sign);
}
