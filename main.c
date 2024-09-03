/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aneumann <aneumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 08:29:16 by roarslan          #+#    #+#             */
/*   Updated: 2024/09/03 15:56:06 by aneumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		if (current->heredoc != NULL)
			printf("  Here_doc name:  %s\n", current->heredoc);
		else
			printf("  Here_doc name not set\n");
		current = current->next;
	}
}

//ajouter un fonction reset pour revenir proprement dans la miniloop sans leaks etc
//par exemple en cas de ctrl+D
//free_data le fait mais il faut voir des cas precis 
void	miniloop(t_data *data)
{
	char	*line;

	while (42)
	{
		line = readline("Minishell> ");
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
			//printtoken(data);
			parser(data);
			expander(data);
			//test du parseur
////////////////			// TA PARTIE COMMENCE ICI!!
			print_cmd_list(data->cmd);
			executor(data);
			
			// exemple de fonctionnement de built_in
			// if (data->cmd->built_in != NULL)
			// 	data->cmd->built_in(data, data->cmd);
			free_cmd_list(data);
		}
	}
}

void	free_data(t_data *data, int exit_code)
{
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
	miniloop(&data);
	rl_clear_history();
	free_var_list(&data);
	// free_cmd_list(&data);
	return (data.exit_code);
}

//to do list
/*  toute l'exec et redirections
	gerer les redirections vides // probablement fini
	peut etre refaire la tokenisation et expansion de variables ex: $PWD-test doit etre colle
	gerer proprement l'exit code
    builtins
    signaux 
*/
