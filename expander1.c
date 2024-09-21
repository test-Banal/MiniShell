/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 15:10:13 by roarslan          #+#    #+#             */
/*   Updated: 2024/09/21 14:41:23 by aneumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	toggle_quote_status(int *in_quotes, char *quote_char, char c)
// {
// 	if (*in_quotes == 0)
// 	{
// 		*in_quotes = 1;
// 		*quote_char = c;
// 	}
// 	else if (c == *quote_char)
// 	{
// 		*in_quotes = 0;
// 		*quote_char = '\0';
// 	}
// }

char	*remove_outer_quotes(char *tmp)
{
	int		k;
	int		j;
	int		in_quotes;
	char	quote_char;
	char	*new_str;

	k = 0;
	j = 0;
	in_quotes = 0;
	quote_char = '\0';
	new_str = ft_calloc(ft_strlen(tmp) + 1, sizeof(char));
	if (!new_str)
		return (NULL);
	while (tmp[k] != '\0')
	{
		if ((tmp[k] == '\'' || tmp[k] == '\"')
			&& (in_quotes == 0 || tmp[k] == quote_char))
		{
			if (in_quotes == 0)
			{
				in_quotes = 1;
				quote_char = tmp[k];
			}
			else if (tmp[k] == quote_char)
			{
				in_quotes = 0;
				quote_char = '\0';
			}
		}
		else
			new_str[j++] = tmp[k];
		k++;
	}
	new_str[j] = '\0';
	return (new_str);
}

void	delete_quotes(t_data *data)
{
	t_cmd	*current;
	int		i;
	char	*tmp;
	char	*new_str;

	current = data->cmd;
	while (current != NULL)
	{
		i = 0;
		while (current->args[i])
		{
			tmp = current->args[i];
			new_str = remove_outer_quotes(tmp);
			free(current->args[i]);
			current->args[i] = new_str;
			i++;
		}
		current = current->next;
	}
}

void	expander(t_data *data)
{
	char	*tmp;
	t_cmd	*current;

	expand_var_quotes(data);
	delete_quotes(data);
	current = data->cmd;
	tmp = NULL;
	while (current != NULL)
	{
		if (current->args[0] && current->args[0][0] != '$')
		{
			tmp = expand_cmd(current->args[0], data);
			if (tmp)
			{
				free(current->args[0]);
				current->args[0] = ft_strdup(tmp);
				free(tmp);
			}
		}
		current = current->next;
	}
}
