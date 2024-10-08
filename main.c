/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 08:29:16 by roarslan          #+#    #+#             */
/*   Updated: 2024/09/24 08:36:53 by roarslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_sig[2];

void	builtins_init(t_data *data)
{
	data->exit_code = 0;
	data->built_in[0] = "cd";
	data->built_in[1] = "echo";
	data->built_in[2] = "env";
	data->built_in[3] = "exit";
	data->built_in[4] = "export";
	data->built_in[5] = "pwd";
	data->built_in[6] = "unset";
	data->built_in[7] = NULL;
	data->built_in_functions[0] = &cd_function;
	data->built_in_functions[1] = &echo_function;
	data->built_in_functions[2] = &env_function;
	data->built_in_functions[3] = &exit_function;
	data->built_in_functions[4] = &export_function;
	data->built_in_functions[5] = &pwd_function;
	data->built_in_functions[6] = &unset_function;
	data->built_in_functions[7] = NULL;
	data->cmd = NULL;
	data->option = EMPTY;
}

void	loop_routine(t_data *data, char *line)
{
	free(line);
	parser(data);
	expander(data);
	executor(data);
	free_pipex(data);
	free_cmd_list(data);
}

int	miniloop(t_data *data)
{
	char	*line;

	while (42)
	{
		globals_init();
		ft_signal(data, EMPTY);
		line = readline(GREEN"[MINISHELL]: "RESET);
		if (g_sig[0] == 1)
			global_c(data);
		if (!line)
			return (set_exit_code(data, 0), \
			write(STDERR_FILENO, "exit\n", 5), 1);
		if (line != NULL || line[0] != '\0')
		{
			add_history(line);
			if (!lexer(line, data))
			{
				free(line);
				continue ;
			}
			loop_routine(data, line);
		}
		ft_signal(data, WRITE);
	}
	return (0);
}

void	free_data(t_data *data, int exit_code)
{
	free_pipex(data);
	free_cmd_list(data);
	set_exit_code(data, exit_code);
	miniloop(data);
}

int	main(int ac, char **av, char **env)
{
	struct s_data	data;

	(void)av;
	(void)ac;
	data.var = NULL;
	data.token = NULL;
	data.cmd = NULL;
	data.redir = NULL;
	data.pipes = NULL;
	data.child_pids = NULL;
	data.pwd = NULL;
	data.old_pwd = NULL;
	printf("\n%s\n", WELCOME);
	builtins_init(&data);
	ft_env_to_lst(&data, env);
	get_pwd(&data);
	miniloop(&data);
	rl_clear_history();
	free_pipex(&data);
	free_var_list(&data);
	free_cmd_list(&data);
	return (data.exit_code);
}
