/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aneumann <aneumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 12:19:32 by aneumann          #+#    #+#             */
/*   Updated: 2024/09/03 14:53:01 by aneumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"minishell.h"

//boucle pour gerer toutes les commandes
bool	ft_open_redir(t_data *data)
{
	t_cmd	*current;

	current = data->cmd;
	while (current->next != NULL)
	{
		if(!ft_open_redir2(current->redirection) == false)
			return (false);
		current = current->next;
	}
	return (true);
}

//boucle pour gerer toutes les redirections de chacune des commandes
bool	ft_open_redir2(t_redir *redirection)
{
	t_redir	*current;

	current = redirection;
	while (current != NULL)
	{
		if (current->id == LESS)
			ft_open_redir_less(current);
		else if (current->id == MORE)
			ft_open_redir_more(current);
		else if (current->id == LESS_LESS)
			ft_open_redir_less_less(current);
		else if (current->id == MORE_MORE)
			ft_open_redir_more_more(current);
		else
			return (false);
		current = current->next;
	}
	return (true);
}

//fonctions pour ouvrir les redirections : >
void	ft_open_redir_more(t_redir *redir)
{
	int	fd;

	fd = open(redir->next->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("open");
		free_data(redir->data, 0);
	}
}

//fonctions pour ouvrir les redirections : >>
void	ft_open_redir_more_more(t_redir *redir) //append
{
	int	fd;

	fd = open(redir->next->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror("open");
		free_data(redir->data, EXIT_FAILURE);
	}
}

//fonctions pour ouvrir les redirections : <
void	ft_open_redir_less(t_redir *redir)
{
	int	fd;

	fd = open(redir->next->str, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		free_data(redir->data, EXIT_FAILURE);
	}   
}

//fonctions pour ouvrir les redirections : << : heredoc

// ALBAN :  PROBLEME HERE_DOC : Ouverture ok masi fermeture impossible ne reconnait pas eof
void	ft_open_redir_less_less(t_redir *redir)
{
	int		fd;
    char	*line;

	fd = open("heredoc85", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("open");
		free_data(redir->data, EXIT_FAILURE);
	}
	//printf("%s", redir->next->str); je n'arrive pas a lire la valeur de redir->next->str
	while (1)
	{
		write(1, " > ", 3);
		line = get_next_line(STDIN_FILENO);
		if (line == NULL ||*line == '\0')
		{
			write(fd, "\n", 1);
			write(1, "bash: warning - file delimited by end-of-file ", 46);
			write(1, redir->next->str, ft_strlen(redir->next->str));
			write(1, "\n", 1);
			break ;
		}
		if ((ft_strcmp(line, redir->next->str) == 0))
			break ;
		write(fd, line, ft_strlen(line));
		free(line);
	}
	free(line);
	close(fd);
	fd = open("heredoc85", O_RDONLY);
	if (fd == -1)
	{
		unlink("heredoc85");
		perror("open");
		free_data(redir->data, 0);
	}
}
