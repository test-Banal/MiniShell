/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 13:17:09 by roarslan          #+#    #+#             */
/*   Updated: 2024/09/03 11:03:52 by roarslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			ft_putstr_fd("syntax error\n", STDERR_FILENO);
			miniloop(data);
			return ;
		}
		current = current->next;
	}
}
