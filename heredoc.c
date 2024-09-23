/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 16:27:46 by aneumann          #+#    #+#             */
/*   Updated: 2024/09/23 13:37:09 by roarslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_open_redir_heredoc(t_redir *redir)
{
	if (redir->cmd->pipe_in != -1 && !isatty(redir->cmd->pipe_in))
		close(redir->cmd->pipe_in);
	redir->cmd->pipe_in = open(redir->next->str, O_RDONLY);
	if (redir->cmd->pipe_in == -1)
	{
		if (redir->next->str)
			ft_putstr_fd(redir->next->str, 2);
		ft_putstr_fd(" heredoc error\n", 2);
	}
}

void	ft_heredoc_search(t_cmd *cmd)
{
	t_redir	*current;
	int		fd;
	char	*line;

	fd = 0;
	line = NULL;
	current = cmd->redirection;
	while (current != NULL)
	{
		if (current->id == LESS_LESS)
			ft_handle_heredoc_helper(current, fd, line);
		current = current->next;
	}
}

void	ft_heredoc_handler(t_data *data)
{
	t_cmd	*current;

	current = data->cmd;
	while (current != NULL)
	{
		if (current->redirection != NULL)
			ft_heredoc_search(current);
		current = current->next;
	}
}

int	ft_handle_heredoc_helper(t_redir *redir, int fd, char *line)
{
	fd = open(redir->next->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (perror("open"), free_data(redir->data, EXIT_FAILURE), 1);
	while (1)
	{
		ft_signal(redir->data, HEREDOC);
		line = readline(" > ");
		if (g_sig[1] == 130)
		{
			g_sig[0] = 0;
			g_sig[1] = 1;
			return (close(fd), 130);
		}
		if (!line || !line[0])
			return (ft_error_heredoc(line, fd, redir->next->str), 1);
		if (line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		if ((ft_strcmp(line, redir->next->str) == 0))
			break ;
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
		ft_signal(redir->data, EMPTY);
	}
	return (free(line), close(fd), 0);
}
