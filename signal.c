/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aneumann <aneumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 15:08:08 by aneumann          #+#    #+#             */
/*   Updated: 2024/09/21 15:35:15 by aneumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/ioctl.h>
#include <termios.h>

void	signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		write(STDERR_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_sig = 1;
	}
	if (signum == SIGQUIT)
	{
		write(STDERR_FILENO, "Quit\n", 5);
		g_sig = 131;
	}
}

void	heredoc_signal(int status)
{
	if (status == SIGINT)
	{
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		g_sig = 130;
	}
}

void	c_signal(int status)
{
	if (status == SIGINT)
	{
		write(STDERR_FILENO, "\n", 1);
		g_sig = 1;
	}
}

void	sigquit_handler(int signum)
{
	const char	*msg;

	if (signum == SIGQUIT)
	{
		msg = "Quit (core dumped)\n";
		write(STDERR_FILENO, msg, ft_strlen(msg));
		g_sig = 1;
	}
}
