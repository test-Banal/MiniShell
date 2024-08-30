/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aneumann <aneumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 12:35:22 by aneumann          #+#    #+#             */
/*   Updated: 2024/08/30 12:35:29 by aneumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// free + exit apres excve
void    ft_free_n_exit_pipex(t_data *data, int exitcode)
{
    if (data->pipes)
        free_pipes(data);
    ft_free_data(data);
    exit(exitcode);
}