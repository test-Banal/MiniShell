/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 14:28:50 by roarslan          #+#    #+#             */
/*   Updated: 2024/08/28 16:46:25 by roarslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/////////////////rajouter une fonction qui gere ||| etc pour $? 2 
int	lexer(char *line, struct s_data *data)
{
	int	start;
	int	end;

	start = 0;
	if (!line)
		return (0);
	if (!check_quotes(line))
	{
		data->exit_code = 2;
		return (write(2, "Error\nUnclosed quotes\n", 22), 0);
	}
	while (line[start] != '\0')
	{
		if (line[start] && is_redirection(line, start))
			lexer_redir(&line, &start, &end, data);
		else if (line[start] && is_word(line, start))
			lexer_word(&line, &start, &end, data);
		else if (line[start] && is_quote(line[start]))
			lexer_quotes(&line, &start, &end, data);
		else if (line[start] && is_whitespace(line[start]))
		start++;
	}
	return (1);
}

void	lexer_redir(char **line, int *start, int *end, t_data *data)
{
	char	*str;
	int		token_id;

	*end = find_end_separator(*line, *start);
	str = ft_substr(*line, *start, *end - *start);
	token_id = find_token_id(str);
	add_token(str, data, token_id);
	*start = *end;
	free(str);
}

void	lexer_word(char **line, int *start, int *end, t_data *data)
{
	char	*str;

	*end = find_end_word(*line, *start);
	str = ft_substr(*line, *start, *end - *start);
	add_token(str, data, WORD);
	*start = *end;
	free(str);
}

void	lexer_quotes(char **line, int *start, int *end, t_data *data)
{
	char	*str;

	*end = find_end_quote(*line, *start + 1);
	str = ft_substr(*line, *start, *end - *start);
	add_token(str, data, WORD);
	*start = *end;
	free(str);
}

int	find_end_quote(char *str, int i)
{
	char	which_quote;

	which_quote = str[i - 1];
	while (str[i])
	{
		if (str[i] == which_quote)
			return (i + 1);
		i++;
	}
	return (i + 1);
}
