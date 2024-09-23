/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 15:34:24 by aneumann          #+#    #+#             */
/*   Updated: 2024/09/23 15:41:33 by roarslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_signal(t_data *data, int option)
{
	data->option = option;
	ft_signal_option(data);
}

void	ft_signal_option(t_data *data)
{
	if (data->option == EMPTY)
	{
		signal(SIGINT, signal_handler);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (data->option == WRITE)
	{
		signal(SIGINT, &c_signal);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (data->option == HEREDOC)
	{
		signal(SIGINT, &heredoc_signal);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (data->option == CHILD)
	{
		signal(SIGINT, &c_signal);
		signal(SIGQUIT, sigquit_handler);
	}
}

void	globals_init(void)
{
	g_sig[0] = 0;
	g_sig[1] = 0;
}

void	global_c(t_data *data)
{
	g_sig[0] = 0;
	set_exit_code(data, 130);
}
