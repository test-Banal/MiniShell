/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 14:28:50 by roarslan          #+#    #+#             */
/*   Updated: 2024/09/19 16:38:54 by roarslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	only_spaces(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (!is_whitespace(line[i]))
			return (0);
		i++;
	}
	return (1);
}

int	lexer(char *line, struct s_data *data)
{
	int	start;
	int	end;

	start = 0;
	if (!line)
		return (0);
	if (!check_quotes(line))
	{
		set_exit_code(data, 2);
		return (write(2, "Error\nUnclosed quotes\n", 22), 0);
	}
	if (only_spaces(line))
		return (0);
	while (line[start] != '\0')
	{
		if (line[start] && is_redirection(line, start))
			lexer_redir(&line, &start, &end, data);
		else if (line[start] && is_word(line, start, 0))
			lexer_word(&line, &start, &end, data);
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
