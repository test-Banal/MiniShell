/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aneumann <aneumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 12:34:13 by aneumann          #+#    #+#             */
/*   Updated: 2024/08/30 12:34:14 by aneumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//creation pipe, allocation pids, fonction exitcode, 


//alocate pid : creer un tableau de pid_t de la taille du nombre de commandes
// et initialise chaque case a -1, pourque le processus parent verifier 
//que le processus enfant a bien ete execute. 
bool    allocate_pids(t_data *data)
{
    int i;

    i = 0;
    data->child_pids = malloc(sizeof(pid_t) * data->size);
    if (!data->child_pids)
        return (false);
    while (i < data->size)
    {
        data->child_pids[i] = -1;
        i++;
    }
    return (true);
}

//attend la fin de chaque processus enfant, pour le processus parent
bool    wait_pids(t_data *data)
{
    int i;
    int status;

    i = 0;
    while (i < data->size)
    {
        waitpid(data->child_pids[i], &status, 0);
        i++;
    }
    handle_exitcode(data, i, status);
    return (true);
}

//exitcode,
void   handle_exitcode(t_data *data, int i, int status)
{
    if (WIFEXITED(status))
    {
        if (WEXITSTATUS(status) == 0)
            data->exit_code = 0;
        else
            data->exit_code = 1;
    }
    else if (WIFSIGNALED(status))
    {
        if (WTERMSIG(status) == 2)
            data->exit_code = 130;
        else if (WTERMSIG(status) == 3)
            data->exit_code = 131;
        else
            data->exit_code = 128 + WTERMSIG(status);
    }
    else
        data->exit_code = 1;
}