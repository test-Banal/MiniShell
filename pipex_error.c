/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aneumann <aneumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 12:35:22 by aneumann          #+#    #+#             */
/*   Updated: 2024/09/03 11:27:09 by aneumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// free + exit apres excve
// ROB on doit pas exit du programme mais plutot lancer la fonction ft_free_data(data)
//qui va vider la liste t_cmd et relancer la loop du minishell
// void	ft_free_n_exit_pipex(t_data *data, int exitcode)
// {
// 	if (data->pipes)
// 		free_pipes(data);
// 	free_data(data, exitcode);
// 	// exit(exitcode);
// }