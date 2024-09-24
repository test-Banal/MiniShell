/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aneumann <aneumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 13:21:26 by roarslan          #+#    #+#             */
/*   Updated: 2024/09/24 08:14:56 by aneumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	syntax_check(t_data *data)
{
	t_cmd	*current;

	current = data->cmd;
	while (current != NULL)
	{
		if (current->args == NULL && current->redirection == NULL)
			return (set_exit_code(data, 2), 0);
		current = current->next;
	}
	if (!check_redir_syntax2(data))
		return (0);
	return (1);
}

int	check_redir_syntax2(t_data *data)
{
	t_cmd	*current_cmd;
	t_redir	*current_redir;
	int		count;

	current_cmd = data->cmd;
	while (current_cmd != NULL)
	{
		current_redir = current_cmd->redirection;
		count = 0;
		while (current_redir != NULL)
		{
			count++;
			current_redir = current_redir->next;
		}
		if (count % 2 != 0)
			return (set_exit_code(data, 2), 0);
		current_cmd = current_cmd->next;
	}
	return (1);
}

void	check_pipes_syntax(t_data *data, t_token *token)
{
	t_token	*current;
	t_token	*head;

	current = token;
	head = data->token;
	while (current != NULL)
	{
		if (current->id == PIPE && ft_strlen(current->str) > 1)
		{
			ft_putstr_fd("syntax error\n", 2);
			free_token_list(head);
			data->token = NULL;
			free_data(data, 2);
			return ;
		}
		current = current->next;
	}
}

void	check_redir_syntax(t_data *data, t_token *token)
{
	t_token	*current;
	t_token	*head;

	current = token;
	head = data->token;
	while (current != NULL)
	{
		if ((current->id == LESS || current->id == LESS_LESS
				|| current->id == MORE || current->id == MORE_MORE)
			&& ft_strlen(current->str) > 2)
		{
			ft_putstr_fd("syntax error\n", 2);
			free_token_list(head);
			data->token = NULL;
			free_data(data, 2);
			return ;
		}
		current = current->next;
	}
}

void	check_empty_commands(t_data *data)
{
	t_token	*current;
	t_token	*head;

	current = data->token;
	head = data->token;
	while (current != NULL)
	{
		if ((current->id == PIPE
				&& (current->next == NULL || current->prev == NULL))
			|| (current->id == PIPE
				&& ((current->next && current->next->id == PIPE)
					|| (current->prev && current->prev->id == PIPE))))
		{
			ft_putstr_fd("syntax error\n", 2);
			free_token_list(head);
			data->token = NULL;
			free_data(data, 2);
			return ;
		}
		current = current->next;
	}
}
