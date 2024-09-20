/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 13:21:26 by roarslan          #+#    #+#             */
/*   Updated: 2024/09/19 16:14:58 by roarslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	syntax_check(t_data *data)
{
	t_cmd	*current;

	current = data->cmd;
	while (current != NULL)
	{
		if (current->args[0] == NULL && current->redirection == NULL)
			return (0);
		current = current->next;
	}
	if (!check_redir_syntax2(data->cmd))
		return (0);
	return (1);
}

int	check_redir_syntax2(t_cmd *cmd)
{
	t_cmd	*current_cmd;
	t_redir	*current_redir;
	int		count;

	current_cmd = cmd;
	while (current_cmd != NULL)
	{
		current_redir = cmd->redirection;
		count = 0;
		while (current_redir != NULL)
		{
			count++;
			current_redir = current_redir->next;
		}
		if (count % 2 != 0)
			return (0);
		current_cmd = current_cmd->next;
	}
	return (1);
}

void	check_pipes_syntax(t_data *data, t_token *token)
{
	t_token	*current;

	current = token;
	while (current != NULL)
	{
		if (current->id == PIPE && ft_strlen(current->str) > 1)
		{
			clean_token_list(data);
			set_exit_code(data, 2);
			ft_putstr_fd("syntax error\n", STDERR_FILENO);
			miniloop(data);
			return ;
		}
		current = current->next;
	}
}

void	check_redir_syntax(t_data *data, t_token *token)
{
	t_token	*current;

	current = token;
	while (current != NULL)
	{
		if ((current->id == LESS || current->id == LESS_LESS
				|| current->id == MORE || current->id == MORE_MORE)
			&& ft_strlen(current->str) > 2)
		{
			clean_token_list(data);
			set_exit_code(data, 2);
			return ;
		}
		current = current->next;
	}
}
