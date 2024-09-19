/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 08:29:16 by roarslan          #+#    #+#             */
/*   Updated: 2024/09/18 21:37:18 by roarslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//variable globale
//g_signals[0] = SIGQUIT ctrl+\
//g_signals[1] = SIGINT ctrl+C
//g_signals[2] = EOF ctrl+D
//a rajouter des cas?
int	global_sig[3];

void	printtoken(t_data *data)
{
	t_token	*current;

	current = data->token;
	while (current != NULL)
	{
		printf("%d token. id = %d. str = %s\n", current->index, current->id, current->str);
		current = current->next;
	}
}

void	builtins_init(t_data *data)
{
	data->pipes = NULL;
	data->child_pids = NULL;
	data->pwd = NULL;
	data->old_pwd = NULL;
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
	global_sig[0] = 0;
	global_sig[1] = 0;
	global_sig[2] = 0;
}

void	print_cmd_list(t_cmd *cmd_list)
{
	t_cmd	*current;
	int		i;

	current = cmd_list;
	while (current != NULL)
	{
		printf("Command:\n");
		if (current->args != NULL)
		{
			printf("  Arguments:\n");
			for (i = 0; current->args[i] != NULL; i++)
				printf("    %s\n", current->args[i]);
		}
		else
			printf("  Arguments: None\n");
		if (current->redirection != NULL)
		{
			printf("  Redirections:\n");
			t_redir	*current_redir = current->redirection;
			while (current_redir != NULL)
			{
				printf("    Redirection: %s\n", current_redir->str);
				current_redir = current_redir->next;
			}
		}
		else
			printf("  Redirections: None\n");
		printf("  pipe_in = %d\n  pipe_out = %d\n", current->pipe_in, current->pipe_out);
		if (current->built_in != NULL)
			printf("  Built-in function: %p\n", current->built_in);
		else
			printf("  Built-in function: None\n");
		printf("  FD = %d\n", current->fd_heredoc);
		current = current->next;
	}
}

//marche pas encore :(
void	sigint_handler(int signum)
{
	(void)signum;
	write(1, "^C", 2);
	global_sig[2] = 1;
}

//ajouter un fonction reset pour revenir proprement dans la miniloop sans leaks etc
//par exemple en cas de ctrl+D
//free_data le fait mais il faut voir des cas precis 
void	miniloop(t_data *data)
{
	char	*line;

	while (42)
	{
		// signal(SIGINT, &sigint_handler);
		line = readline("[Minishell]: ");
		// if (global_sig[2] == 1)
		// {
		// 	global_sig[2] = 0;
		// 	continue ;
		// }
		// signal(SIGINT, SIG_DFL);
		if (line != NULL)
		{
			if (line[0] == '\0')
				continue ;
			add_history(line);
			if (!lexer(line, data))
			{
				free(line);
				continue ;
			}
			free(line);
			// printtoken(data);
			parser(data);
			expander(data);
			// print_cmd_list(data->cmd);
			executor(data);
			free_pipex(data);
			free_cmd_list(data);
		}
	}
}

void	free_pipex(t_data *data)
{
	int	i;

	i = 0;
	if (data->pipes)
	{
		while (i < ft_cmd_lstsize(data->cmd))
		{
			if (data->pipes[i])
				free(data->pipes[i]);
			i++;
		}
		free(data->pipes);
		data->pipes = NULL;
	}
	if (data->child_pids)
	{
		free(data->child_pids);
		data->child_pids = NULL;
	}
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
	printf("\n%s\n", WELCOME);
	builtins_init(&data);
	ft_env_to_lst(&data, env);
	get_pwd(&data);
	miniloop(&data);
	rl_clear_history();
	free_var_list(&data);
	free_cmd_list(&data);
	return (data.exit_code);
}

//to do list
/* 
	gestion d'exit_code pas top -> command not found ne change pas de valeur d'exit code dans le parent
	checker les commandes vides!!!
	refonte de la fonction handle_exitcode();
	dans le parser checker id de la redirection par exemple < <,
	accesoirement refaire le syntax check des redirections
	affichage different entre export truc et export truc=   ?
    signaux 
*/
