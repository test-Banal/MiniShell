/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 15:08:29 by roarslan          #+#    #+#             */
/*   Updated: 2024/08/30 15:47:39 by roarslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_cmd(t_cmd **cmd, t_token *token, t_data *data)
{
	int	size;

	size = 0;
	(*cmd) = malloc(sizeof(t_cmd));
	if (!*cmd)
		return ;
	(*cmd)->built_in = NULL;
	(*cmd)->next = NULL;
	(*cmd)->prev = NULL;
	(*cmd)->pipe_in = 0;
	(*cmd)->pipe_out = 1;
	(*cmd)->redirection = NULL;
	(*cmd)->heredoc = NULL;
	(*cmd)->data_p = data;
	size = get_cmd_size(token);
	(*cmd)->args = ft_calloc(size + 1, sizeof(char *));
	if (!(*cmd)->args)
	{
		free(*cmd);
		*cmd = NULL;
		return ;
	}
}

void	clean_token_list(t_data *data)
{
	t_token	*current;
	t_token	*next;

	current = data->token;
	while (current != NULL && current->id != PIPE)
	{
		next = current->next;
		free_token(current);
		current = next;
	}
	if (current && current->id == PIPE)
	{
		data->token = current->next;
		free_token(current);
	}
	else
		data->token = NULL;
}

int	get_cmd_size(void *lst)
{
	t_token	*current;
	int		len;

	len = 0;
	current = (t_token *)lst;
	while (current != NULL && current->id != PIPE)
	{
		len++;
		current = current->next;
	}
	return (len);
}

void	add_argument(char **args, char *str, int *index)
{
	args[*index] = ft_strdup(str);
	args[*index + 1] = NULL;
	(*index)++;
}

void	free_token(t_token *token)
{
	if (token)
	{
		if (token->str)
			free(token->str);
		free(token);
	}
}
