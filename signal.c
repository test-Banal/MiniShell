# include "minishell.h"
#include <sys/ioctl.h>
#include <termios.h>

// int g_exi_code = 0;

void ft_signal(t_data *data, int option)
{
    data->option = option;
    ft_signal_option(data);
}

void    ft_signal_option(t_data *data)
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

void	signal_handler(int signum)
{
	if (signum == SIGINT) //Ctrl-C
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
