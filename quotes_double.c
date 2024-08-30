/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_double.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 17:06:09 by roarslan          #+#    #+#             */
/*   Updated: 2024/08/26 12:03:10 by roarslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin_char(char *str, char c)
{
	char	*dest;
	int		i;

	if (!str)
		return (NULL);
	dest = malloc(sizeof(char) * (ft_strlen(str) + 2));
	if (!dest)
		return (NULL);
	i = 0;
	while (str[i])
	{
		dest[i] = str[i];
		i++;
	}
	dest[i] = c;
	i++;
	dest[i] = '\0';
	return (dest);
}

int	find_end_var_quote(char *str, int i)
{
	int	len;

	i++;
	len = 0;
	while (str[i] && ((str[i] >= 'A' && str[i] <= 'Z')
			|| (str[i] >= 'a' && str[i] <= 'z')
			|| (str[i] >= '0' && str[i] <= '9')))
	{
		i++;
		len++;
	}
	return (len);
}

// char    *ft_substr_quotes(char *str, int i, t_data *data)
// {
//     char    *dest;
//     char    *var;
//     int     len;

//     dest = ft_strdup("");
//     len = 0;
//     while (str[i])
//     {
//         if (str[i] && str[i] == '$')
//         {
//             len = find_end_var_quote(str, i);
//             var = is_var(str, i, data, len);
//             printf("var = %s\n", var);
//             if (var)
//                 dest = ft_strjoin(dest, var);
//             i += len + 1;
//         }
//         else
//         {
//             dest = ft_strjoin_char(dest, str[i]);
//             i++;
//         }
//         if (str[i] && str[i] == '"')
//             break ;
//     }
//     return (dest);
// }

// char    *ft_substr_quotes(char *str, int i, t_data *data)
// {
//     char    *dest;
//     char    *var;
//     int     len;

//     dest = ft_strdup("");
//     len = 0;
//     while (str[i])
//     {
//         if (str[i] && str[i] == '$')
//         {
//             len = find_end_var_quote(str, i);
//             var = is_var(str, i, data, len);
//             if (var)
//                 dest = ft_strjoin(dest, var);
//             i += len + 1;
//         }
//         else
//         {
//             dest = ft_strjoin_char(dest, str[i]);
//             i++;
//         }
//         if (str[i] && str[i] == '"')
//             break ;
//     }
//     return (dest);
// }
