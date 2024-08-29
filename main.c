/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roarslan <roarslan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 08:29:16 by roarslan          #+#    #+#             */
/*   Updated: 2024/08/27 15:08:47 by roarslan         ###   ########.fr       */
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
		// Print arguments
		if (current->args != NULL)
		{
			printf("  Arguments:\n");
			for (i = 0; current->args[i] != NULL; i++)
				printf("    %s\n", current->args[i]);
		}
		else
			printf("  Arguments: None\n");
		// Print redirections
		if (current->redirection != NULL)
		{
			printf("  Redirections:\n");
			t_redir *current_redir = current->redirection;
			while (current_redir != NULL)
			{
				printf("    Redirection: %s\n", current_redir->str);
				current_redir = current_redir->next;
			}                
		}
		else
			printf("  Redirections: None\n");
		// Print pipe status
		printf("  Pipe: %s\n", current->pipe ? "Yes" : "No");
		// Print built-in status
		if (current->built_in != NULL)
			printf("  Built-in function: %p\n", current->built_in);
		else
			printf("  Built-in function: None\n");
		// Move to the next command in the list
		current = current->next;
	}
}

void	miniloop(t_data *data)
{
	char	*line;

	while (42)
	{
		line = readline("Minishell> ");
		if (line != NULL)
		{
			if (line[0] == '\0')
				miniloop(data);
			add_history(line);
			if (!lexer(line, data))
			{
				free(line);
				miniloop(data);
			}
			free(line);
			parser(data);
			expander(data);
			print_cmd_list(data->cmd);
			if (data->cmd->args != NULL && ft_strcmp(data->cmd->args[0], "exit") == 0)
				break ;
		}
	}
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
	ft_env_to_lst(&data, env);
	builtins_init(&data);
	miniloop(&data);
	//une fonction pour free();
	rl_clear_history();
	free_var_list(&data);
	free_cmd_list(&data);
	return (0);
}


//to do list
/*  toute l'exec et redirections
    builtins
    signaux 
*/
