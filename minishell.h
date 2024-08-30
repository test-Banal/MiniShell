/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aneumann <aneumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 11:43:33 by roarslan          #+#    #+#             */
/*   Updated: 2024/08/30 10:21:01 by aneumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <readline/readline.h> 
# include <readline/history.h>
# include <sys/stat.h>
# include <sys/types.h> 
# include <stdbool.h>
# include <string.h>

# define WELCOME "\n\033[0;32m███╗   ███╗██╗███╗   ██╗██╗███████╗██╗  ██╗███████╗██╗     ██╗     \n\
████╗ ████║██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║     \n\
██╔████╔██║██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║     \n\
██║╚██╔╝██║██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║     \n\
██║ ╚═╝ ██║██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗\n\
╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\033[0m\n\n"

# define PIPE 1
# define LESS 2
# define MORE 3
# define LESS_LESS 4
# define MORE_MORE 5
# define WORD 6

typedef struct s_token
{
	int				index;
	int				id;
	char			*str;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_redir
{
	int				index;
	int				id;
	char			*str;
	char			*path;
	struct s_redir	*next;
	struct s_redir	*prev;
}	t_redir;

typedef struct s_var
{
	int				id;
	char			*name;
	char			*value;
	struct s_var	*next;
	struct s_var	*prev;
}	t_var;

typedef struct s_cmd
{										
	char			**args;
	void			*built_in;
	int				pipe;
	struct s_redir	*redirection;
	struct s_cmd	*next;
	struct s_cmd	*prev;
}	t_cmd;

//besoin de rajouter int infile et int outfile (fichier dentre/sortie/redirection)	
// mais est ce que infile et outfile sont des variables de redir ou de data	/ ou cmd ??
// genre si il y a un fichier d'entre ou est savariable ?

//int start is used in expander for norm
typedef struct s_data
{
	char			*built_in[8];
	void			(*built_in_functions[8])(struct s_data *data);
	struct s_token	*token;
	struct s_var	*var;
	struct s_cmd	*cmd;
	struct s_redir	*redir;
	int				exit_code;
	int				start;
	int				size; //oeoeoe	
	int				**pipes; //oeoeoe  
	int				*child_pids; //oeoeoe
	int 			exitcode; //oeoeoe
}	t_data;

//main
int				find_end_word(char *str, int i);
void			builtins_init(t_data *data);
int				find_end_var(char *str, int i);
void			print_cmd_list(t_cmd *cmd_list); ////////test
void			printtoken(t_data *data); // test
void			miniloop(t_data *data);

//utils1
int				ft_strncmp(const char *s1, const char *s2, size_t n);
char			*ft_strjoin(char *s1, char const *s2);
char			*ft_strcat(char *s1, char *s2);
int				ft_lstsize(t_var *var);
void			free_var_list(t_data *data);
char			*ft_strjoin2(char *s1, char const *s2);

//utils2
void			free_redir_list(t_redir *redir);
void			free_cmd_list(t_data *data);
char			*ft_substr(char const *s, unsigned int start, size_t len);
char			*ft_strdup(const char *s);
size_t			ft_strlen(const char *str);

//utils3
int				ft_strcmp(char *s1, char *s2);
void			*ft_calloc(size_t nmemb, size_t size);
char			*ft_strchr(const char *s, int c);
char			*ft_strjoin_free(char *s1, char *s2);
void			ft_putstr_fd(char *str, int fd);

//parsing0
int				is_pipe(char c);
int				is_whitespace(char c);
int				is_separator(char c);
int				is_word(char *str, int i);
int				is_redirection(char *str, int i);

//parsing1
int				find_end_word(char *str, int i);
int				find_end_separator(char *str, int i);
int				find_token_id(char *str);
int				check_quotes(char *str);
void			add_token(char *str, t_data *data, int token_id);

//parsing2
int				is_single_quote(char c);
int				is_double_quote(char c);
int				is_quote(char c);

//split
void			free_mem(char **tofree);
int				ft_countwords(char const *str, char c);
char			*ft_strduplicate(char const *str, char c);
int				todup(char **dest, char const *s, char c);
char			**ft_split(char const *s, char c);

////env
int				ft_env_to_lst(t_data *data, char **env);
char			*get_name(char *str);
char			*get_value(char *str);
struct s_var	*init_env_var(char *name, char *value, int i);
void			add_var_list(t_data *data, struct s_var *var);
char			**var_list_to_tab(t_var *var);
char			*is_var(char *str, int i, t_data *data, int end);
int				var_lenght(char *str, t_data *data);

//is_cmd
int				is_cmd(char *str, t_data *data);
int				test_path(char *str, char **bins);
int				test_absolute_path(char *str);
char			**get_binarys(t_data *data);
char			*get_path(t_var *var);

//quotes_double
char			*ft_strjoin_char(char *str, char c);
char			*ft_substr_quotes(char *str, int i, t_data *data);

//built_in //rajouter structure t_cmd *cmd a chaque builtin
void			cd_function(t_data *data, t_cmd *cmd);
void			echo_function(t_data *data, t_cmd *cmd);
void			env_function(t_data *data, t_cmd *cmd);
void			exit_function(t_data *data, t_cmd *cmd);
void			export_function(t_data *data, t_cmd *cmd);
void			pwd_function(t_data *data, t_cmd *cmd);
void			unset_function(t_data *data, t_cmd *cmd);
bool			builtin_selection(t_data *data, int i); //rajout fonction builtin

//lexer
int				lexer(char *line, struct s_data *data);
void			lexer_redir(char **line, int *start, int *end, t_data *data);
int				find_end_single(char *str, int i);
void			lexer_word(char **line, int *start, int *end, t_data *data);
void			lexer_quotes(char **line, int *start, int *end, t_data *data);

//parser1
void			parser(t_data *data);
void			parser_helper(t_token **current, t_cmd **cmd, int *index);
void			add_redirection(t_cmd *cmd, t_token *token);
void			append_cmd(t_cmd **cmd_list, t_cmd *new_cmd);
void			*assign_builtin_pointer(t_data *data, char *str);

//parser2
void			init_cmd(t_cmd **cmd, t_token *token);
void			clean_token_list(t_data *data);
int				get_cmd_size(void *lst);
void			add_argument(char **args, char *str, int *index);
void			free_token(t_token *token);

//parser3
void			check_pipes_syntax(t_data *data, t_token *token);
void			check_redir_syntax(t_data *data, t_token *token);

//expander1
void			expander(t_data *data);
char			*expand_cmd(char *str, t_data *data);
char			*find_command_path(char *str, char **bins);
int				find_end_var(char *str, int i);
void			expand_var(t_data *data);

//expander2
char			*change_var(t_data *data, char *var_name);
void			check_for_var2(t_data *data, char **dest, int *i, char *str);
void			check_for_var(t_data *data, char *str, t_cmd *current, int j);
void			expand_var_quotes(t_data *data);
void			expand_var(t_data *data);

//exec


#endif
