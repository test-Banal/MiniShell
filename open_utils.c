/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 12:19:32 by aneumann          #+#    #+#             */
/*   Updated: 2024/09/21 21:53:51 by roarslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"minishell.h"

bool	ft_open_redir2(t_redir *redirection)
{
	t_redir	*current;

	current = redirection;
	while (current != NULL)
	{
		if (current->id == LESS)
			ft_open_redir_input(current);
		else if (current->id == MORE)
			ft_open_redir_output(current);
		else if (current->id == LESS_LESS)
			ft_open_redir_heredoc(current);
		else if (current->id == MORE_MORE)
			ft_open_redir_append(current);
		current = current->next;
	}
	return (true);
}

//fonctions pour ouvrir les redirections : >
void	ft_open_redir_output(t_redir *redir)
{
	if (redir->cmd->pipe_out != -1 && !isatty(redir->cmd->pipe_out))
		close(redir->cmd->pipe_out);
	redir->cmd->pipe_out = open(redir->next->str, \
		O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (redir->cmd->pipe_out == -1)
	{
		if (redir->next->str)
			ft_putstr_fd(redir->next->str, 2);
		ft_putstr_fd(" : Error\n", 2);
	}
}

//fonctions pour ouvrir les redirections : >>
void	ft_open_redir_append(t_redir *redir)
{
	if (!isatty(redir->cmd->pipe_out))
		close(redir->cmd->pipe_out);
	redir->cmd->pipe_out = open(redir->next->str, \
		O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (redir->cmd->pipe_out < 0)
	{
		if (redir->next->str)
			ft_putstr_fd(redir->next->str, 2);
		ft_putstr_fd(" : Error\n", 2);
	}
}

//fonctions pour ouvrir les redirections : <
bool	ft_open_redir_input(t_redir *redir)
{
	if (redir->cmd->pipe_in != -1 && !isatty(redir->cmd->pipe_in))
		close(redir->cmd->pipe_in);
	if (redir->next->str)
		redir->cmd->pipe_in = open(redir->next->str, O_RDONLY);
	if (redir->cmd->pipe_in == -1)
	{
		if (redir->next->str)
			ft_putstr_fd(redir->next->str, 2);
		ft_putstr_fd(" : No such file or directory\n", 2);
	}
	return (true);
}
