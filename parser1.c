/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 14:52:34 by roarslan          #+#    #+#             */
/*   Updated: 2024/09/03 10:43:19 by roarslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parser(t_data *data)
{
	t_token	*current;
	t_cmd	*cmd;
	int		index;

	current = data->token;
	check_pipes_syntax(data, current);
	check_redir_syntax(data, current);
	init_cmd(&cmd, current, data);
	if (!cmd)
		return ;
	index = 0;
	parser_helper(&current, &cmd, &index);
	if (cmd->args != NULL && cmd->args[0] != NULL)
		cmd->built_in = assign_builtin_pointer(data, cmd->args[0]);
	// if (current != NULL && current->id == PIPE)
	// {
	// 	cmd->pipe = 1;
	// 	current = current->next;
	// }
	append_cmd(&data->cmd, cmd);
	clean_token_list(data);
	check_redir_syntax2(data, data->cmd);
	if (data->token != NULL)
		parser(data);
}

void	check_redir_syntax2(t_data *data, t_cmd *cmd)
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
		{
			set_exit_code(data, 2);
			free_cmd_list(data);
			miniloop(data);
			return ;
		}
		current_cmd = current_cmd->next;
	}
}

void	parser_helper(t_token **current, t_cmd **cmd, int *index)
{
	while ((*current) != NULL && (*current)->id != PIPE)
	{
		if ((*current)->id != PIPE && (*current)->id != WORD)
		{
			add_redirection(*cmd, *current);
			*current = (*current)->next;
			if (*current != NULL && (*current)->id != PIPE && (*current)->id == WORD)
			{
				add_redirection(*cmd, *current);
				*current = (*current)->next;
			}
		}
		else if (*current != NULL && (*current)->id == WORD)
		{
			add_argument((*cmd)->args, (*current)->str, index);
			*current = (*current)->next;
		}
	}
}

void	add_redirection(t_cmd *cmd, t_token *token)
{
	t_redir	*new;
	t_redir	*last;

	last = NULL;
	new = malloc(sizeof(t_redir));
	if (!new)
		return ;
	new->index = token->index;
	new->id = token->id;
	new->str = ft_strdup(token->str);
	new->path = getcwd(NULL, 0);///////////////// a tester, peut etre chercher $PWD a la place
	new->data = cmd->data_p;
	new->next = NULL;
	new->prev = NULL;
	if (cmd->redirection == NULL)
		cmd->redirection = new;
	else
	{
		last = cmd->redirection;
		while (last->next != NULL)
			last = last->next;
		last->next = new;
		new->prev = last;
	}
}

void	append_cmd(t_cmd **cmd_list, t_cmd *new_cmd)
{
	t_cmd	*current;

	current = NULL;
	if (*cmd_list == NULL)
		*cmd_list = new_cmd;
	else
	{
		current = *cmd_list;
		while (current->next != NULL)
			current = current->next;
		current->next = new_cmd;
		new_cmd->prev = current;
	}
}

void	*assign_builtin_pointer(t_data *data, char *str)
{
	int	i;

	i = 0;
	while (data->built_in[i] != NULL)
	{
		if (ft_strcmp(data->built_in[i], str) == 0)
			return ((void *)data->built_in_functions[i]);
		i++;
	}
	return (NULL);
}
