/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 12:33:06 by roarslan          #+#    #+#             */
/*   Updated: 2024/09/23 16:19:05 by roarslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	export_error(t_data *data, char *value)
{
	ft_putstr_fd("export : ", 2);
	ft_putstr_fd(value, 2);
	ft_putstr_fd(" : not a valid identifier\n", 2);
	if (value)
		free(value);
	free_data(data, 1);
}

void	ft_dup2_error(t_data *data)
{
	ft_close_fds_child(data);
	free_pipex(data);
	set_exit_code(data, EXIT_FAILURE);
	free_var_list(data);
	free_cmd_list(data);
	exit(EXIT_FAILURE);
}

void	ft_error_heredoc(char *line, int fd, char *str)
{
	write(2, "\nbash: warning - file delimited by end-of-file ", 46);
	write(2, str, ft_strlen(str));
	write(2, "\n", 1);
	free(line);
	close(fd);
}

void	ft_execve_dir_error(char *str, t_data *data)
{
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": Is a directory\n", 2);
	free_pipex(data);
	set_exit_code(data, 126);
	free_var_list(data);
	free_cmd_list(data);
	exit(126);
}
