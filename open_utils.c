/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aneumann <aneumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 12:19:32 by aneumann          #+#    #+#             */
/*   Updated: 2024/09/19 15:05:42 by aneumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"minishell.h"
#include <sys/ioctl.h>
#include <termios.h>

volatile sig_atomic_t g_heredoc_interrupted = 0;
#include <setjmp.h>

jmp_buf jump_buffer; // Declare the 'jump_buffer' variable

///////TENTER UN AUTRE TRUC POUR LES REDIRECTIONS MAIS POUR L'INSTANT CE NE FONCTIONNE TOUJOURS PAS, sinon pb de here-doc regle mais tjrs pas redirection

//boucle pour gerer toutes les commandes
bool	ft_open_redir(t_data *data)
{
	t_cmd	*current;

	current = data->cmd;
	while (current != NULL)
	{
		if (current->redirection != NULL)
		{
			if(ft_open_redir2(current->redirection) == false)
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
		// if (current->next != NULL)
		// 	printf("REDIR : %s\n", current->next->str);
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
	if (!isatty(redir->cmd->pipe_out))
		close(redir->cmd->pipe_out);	
	redir->cmd->pipe_out = open(redir->next->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (redir->cmd->pipe_out == -1)
	{
		ft_putstr_fd(redir->next->str, 2);
		ft_putstr_fd("Error\n", 2);
		// perror("open output");
		// free_data(redir->data, EXIT_FAILURE);
	}
	else
		redir->cmd->file = true;
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
//fd ouvert 
void	ft_open_redir_append(t_redir *redir)
{
	if (!isatty(redir->cmd->pipe_out))
		close(redir->cmd->pipe_out);
	redir->cmd->pipe_out = open(redir->next->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (redir->cmd->pipe_out < 0)
	{
		ft_putstr_fd(redir->next->str, 2);
		ft_putstr_fd("Error\n", 2);// ajouter commentaire ? 
		// perror("open append");
		// free_data(redir->data, EXIT_FAILURE);
	}
	else
		redir->cmd->file = true;
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
	if (!isatty(redir->cmd->pipe_in))
		close(redir->cmd->pipe_in);
	redir->cmd->pipe_in = open(redir->next->str, O_RDONLY);
	if (redir->cmd->pipe_in == -1)
	{
		ft_putstr_fd(redir->next->str, 2);
		ft_putstr_fd("No such file or directory\n", 2);
		// perror("open input");
		// free_data(redir->data, EXIT_FAILURE);
	}
	else
		redir->cmd->file = true;
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

void	ft_open_redir_heredoc(t_redir *redir)
{
	if (!isatty(redir->cmd->pipe_in))
		close(redir->cmd->pipe_in);
	redir->cmd->pipe_in = open(redir->next->str, O_RDONLY);
	if (redir->cmd->pipe_in == -1)
	{
		ft_putstr_fd(redir->next->str, 2);
		ft_putstr_fd("No such file or directory\n", 2);
		// perror("open input");
		// free_data(redir->data, EXIT_FAILURE);
	}
	else
		redir->cmd->file = true;
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

//changer de nom du heredoc!!!
void	ft_handle_heredoc_helper(t_redir *redir, int fd, char *line)
{
	fd = open(redir->next->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("open");
		free_data(redir->data, EXIT_FAILURE);
	}
	// signal(SIGINT, &heredoc_signal);
	// signal(SIGQUIT, SIG_IGN);
	if (setjmp(jump_buffer) == 0)
	{
		while (1)
		{
			signal(SIGINT, &heredoc_signal);
			write(1, " > ", 3);

			signal(SIGQUIT, SIG_IGN);
			line = get_next_line(STDIN_FILENO);
			if (!line)
			{
				write(2, "\nbash: warning - file delimited by end-of-file ", 46);
				write(2, redir->next->str, ft_strlen(redir->next->str));
				write(2, "\n", 1);
				break ;
			}
			// if (g_heredoc_interrupted == 1)
			// {
			// 	free_data(redir->data, 0);
			// 	break;
			// }

			if (line[ft_strlen(line) - 1] == '\n')
				line[ft_strlen(line) - 1] = '\0';
			if ((ft_strcmp(line, redir->next->str) == 0))
				break;
			write(fd, line, ft_strlen(line));
			write(fd, "\n", 1);
			free(line);
		}
		free(line);
		close(fd);
		fd = open(redir->next->str, O_RDONLY);
		if (fd == -1)
		{
			unlink(redir->next->str);
			perror("open");
			free_data(redir->data, 0);
		}
	//supprimer le fichier tmp .heredoc85
	// redir->cmd->fd_heredoc = fd;
		close(fd);
	}
}


void	heredoc_signal(int status)
{
	if (status == SIGINT)
		{
			g_heredoc_interrupted = 1;
			write(STDERR_FILENO, "\n", 1);
			longjmp(jump_buffer, 1); 
		}
}