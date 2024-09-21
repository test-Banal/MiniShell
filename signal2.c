/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aneumann <aneumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 15:34:24 by aneumann          #+#    #+#             */
/*   Updated: 2024/09/21 15:36:43 by aneumann         ###   ########.fr       */
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
