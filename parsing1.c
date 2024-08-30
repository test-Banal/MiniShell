/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 13:52:39 by roarslan          #+#    #+#             */
/*   Updated: 2024/08/27 13:52:17 by roarslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_end_word(char *str, int i)
{
	while (str[i] && is_word(str, i))
	{
		i++;
		if ((str[i - 1] && str[i - 1] != '$') && str[i] == '$')
			break ;
	}
	if (!str[i])
		return (i);
	return (i);
}

int	find_end_separator(char *str, int i)
{
	while (str[i] && is_separator(str[i]))
		i++;
	return (i);
}

int	find_token_id(char *str)
{
	if (ft_strncmp(str, "|", 1) == 0)
		return (PIPE);
	else if (ft_strncmp(str, "<<", 2) == 0)
		return (LESS_LESS);
	else if (ft_strncmp(str, ">>", 2) == 0)
		return (MORE_MORE);
	else if (ft_strncmp(str, "<", 1) == 0)
		return (LESS);
	else if (ft_strncmp(str, ">", 1) == 0)
		return (MORE);
	return (0);
}

int	check_quotes(char *str)
{
	int	i;
	int	single_quotes;
	int	double_quotes;

	i = 0;
	single_quotes = 0;
	double_quotes = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			single_quotes++;
		if (str[i] == '"')
			double_quotes++;
		i++;
	}
	if ((single_quotes % 2 != 0) || (double_quotes % 2 != 0))
		return (0);
	return (1);
}

void	add_token(char *str, t_data *data, int token_id)
{
	t_token	*current;
	t_token	*new;

	current = NULL;
	if (!str || token_id == 0)
		return ;
	new = (t_token *)malloc(sizeof(t_token));
	if (!new)
		return ;
	new->index = 0;
	new->id = token_id;
	new->str = ft_strdup(str);
	new->next = NULL;
	new->prev = NULL;
	if (!data->token)
		data->token = new;
	else
	{
		current = data->token;
		while (current->next != NULL)
			current = current->next;
		current->next = new;
		new->prev = current;
		new->index = current->index + 1;
	}
}
