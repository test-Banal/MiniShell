/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing0.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 18:17:09 by roarslan          #+#    #+#             */
/*   Updated: 2024/08/24 16:11:31 by roarslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_pipe(char c)
{
	if (c == '|')
		return (1);
	return (0);
}

int	is_whitespace(char c)
{
	if (c == 32 || c == 9 || c == 10 || c == 11
		|| c == 12 || c == 13)
		return (1);
	return (0);
}

int	is_separator(char c)
{
	if (c == '<' || c == '>' || c == '|')
		return (1);
	return (0);
}

int	is_word(char *str, int i)
{
	if (!(is_quote(str[i])) && !(is_whitespace(str[i]))
		&& !(is_separator(str[i])))
		return (1);
	return (0);
}

int	is_redirection(char *str, int i)
{
	if (str[i] && is_separator(str[i]))
		return (1);
	return (0);
}
