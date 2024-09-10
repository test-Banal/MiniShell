/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aneumann <aneumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 12:19:32 by aneumann          #+#    #+#             */
/*   Updated: 2024/09/05 15:11:00 by aneumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"minishell.h"

///////TENTER UN AUTRE TRUC POUR LES REDIRECTIONS MAIS POUR L'INSTANT CE NE FONCTIONNE TOUJOURS PAS, sinon pb de here-doc regle mais tjrs pas redirection

//boucle pour gerer toutes les commandes
bool	ft_open_redir(t_data *data)
{
	t_cmd	*current;

	current = data->cmd;
	while (current->next != NULL)
	{
		if (current->redirection != NULL)
		{
			if(!ft_open_redir2(current->redirection) == false)
				return (false);
		}
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
			ft_open_redir_input(current);
		else if (current->id == MORE)
			ft_open_redir_output(current);
		else if (current->id == LESS_LESS)
			ft_open_redir_heredoc(current);
		else if (current->id == MORE_MORE)
			ft_open_redir_append(current);
		else
			return (false);
		current = current->next->next;
		printf("%s\n", current->next->str);
	}
	return (true);
}

//fonctions pour ouvrir les redirections : >
void	ft_open_redir_output(t_redir *redir)
{
	if (redir->cmd->output != 1 && redir->cmd->output != STDOUT_FILENO)
		close(redir->cmd->output);	
	redir->cmd->output = open(redir->next->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (redir->cmd->output == -1)
	{
		perror("open output");
		free_data(redir->data, EXIT_FAILURE);
	}
}
// {
// 	int	fd;

// 	fd = open(redir->next->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	if (fd == -1)
// 	{
// 		perror("open");
// 		free_data(redir->data, 0);
// 	}
// 	redir->cmd->fd = fd;
// }


//fonctions pour ouvrir les redirections : >>
void	ft_open_redir_append(t_redir *redir)
{
	if (redir->cmd->output != STDOUT_FILENO && redir->cmd->output != -1)
		close(redir->cmd->output);
	redir->cmd->output = open(redir->next->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (redir->cmd->output < 0)
	{
		perror("open append");
		free_data(redir->data, EXIT_FAILURE);
	}
}

// {
// 	int	fd;

// 	fd = open(redir->next->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 	if (fd == -1)
// 	{
// 		perror("open");
// 		free_data(redir->data, EXIT_FAILURE);
// 	}
// 	redir->cmd->fd = fd;
// }

//fonctions pour ouvrir les redirections : <
void	ft_open_redir_input(t_redir *redir)
{
	if (redir->cmd->input != 0 && redir->cmd->input != STDIN_FILENO)
		close(redir->cmd->input);
	redir->cmd->input = open(redir->next->str, O_RDONLY);
	if (redir->cmd->input == -1)
	{
		perror("open input");
		free_data(redir->data, EXIT_FAILURE);
	}
}
// {
// 	int	fd;

// 	fd = open(redir->next->str, O_RDONLY);
// 	if (fd == -1)
// 	{
// 		perror("open");
// 		free_data(redir->data, EXIT_FAILURE);
// 	}
// 	redir->cmd->fd = fd;
// }

//fonctions pour ouvrir les redirections : << : heredoc
// ALBAN :  PROBLEME HERE_DOC : Ouverture ok masi fermeture impossible ne reconnait pas eof
void	ft_open_redir_heredoc(t_redir *redir)
{
	int		fd;
    char	*line;

	fd = open("heredoc85", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("open");
		free_data(redir->data, EXIT_FAILURE);
	}
	while (1)
	{
		write(1, " > ", 3);
		line = get_next_line(STDIN_FILENO);
		if (!line) // || *line == '\0')
		{
			write(fd, "\n", 1);
			write(1, "bash: warning - file delimited by end-of-file ", 46);
			write(1, redir->next->str, ft_strlen(redir->next->str));
			write(1, "\n", 1);
			break ;
		}
		if (line[ft_strlen(line) - 1] == '\n')
		 	line[ft_strlen(line) - 1] = '\0';
		if  ((ft_strcmp(line, redir->next->str) == 0))
			break ;
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	free(line);
	close(fd);
	fd = open(".heredoc.tmp", O_RDONLY);
	if (fd == -1)
	{
		unlink(".heredoc.tmp");
		perror("open");
		free_data(redir->data, 0);
	}
	redir->cmd->fd = fd;
}
