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

void   handle_exitcode(t_data *data, int i, int status)
{
    if (WIFEXITED(status))
    {
        if (WEXITSTATUS(status) == 0)
            data->exitcode = 0;
        else
            data->exitcode = 1;
    }
    else if (WIFSIGNALED(status))
    {
        if (WTERMSIG(status) == 2)
            data->exitcode = 130;
        else if (WTERMSIG(status) == 3)
            data->exitcode = 131;
        else
            data->exitcode = 128 + WTERMSIG(status);
    }
    else
        data->exitcode = 1;
}
