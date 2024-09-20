/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 12:33:06 by roarslan          #+#    #+#             */
/*   Updated: 2024/09/20 12:33:29 by roarslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	export_error(t_data *data, char *value)
{
	ft_putstr_fd("export : ", 2);
	ft_putstr_fd(value, 2);
	ft_putstr_fd(" : not a valid identifier\n", 2);
	if (value)
		free(value);
	free_data(data, 1);
}
