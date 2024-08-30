#include "minishell.h"

// free + exit apres excve
void    ft_free_n_exit_pipex(t_data *data, int exitcode)
{
    if (data->pipes)
        free_pipes(data);
    ft_free_data(data);
    exit(exitcode);
}