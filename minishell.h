/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aneumann <aneumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 11:43:33 by roarslan          #+#    #+#             */
/*   Updated: 2024/09/19 15:16:07 by aneumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <readline/readline.h> 
# include <stdbool.h>
# include <readline/history.h>
# include <sys/stat.h>
# include <sys/types.h> 
# include <sys/wait.h>
# include <string.h>
# include <limits.h>
# include <signal.h>

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

# define GREEN "\033[0;32m"
# define RESET "\033[0m"
 
 # define CHILD 2

//for get_next_line
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

typedef struct s_data t_data;
typedef struct s_cmd t_cmd;
typedef void (*t_b_in)(t_data *data, t_cmd *cmd);

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
	struct s_data	*data;
	struct s_cmd	*cmd;
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
	t_b_in			built_in;
	int				pipe_in;
	int				pipe_out;
	char			*heredoc;
	bool			file;
	int				fd_heredoc; // probablement pas besoin
	struct s_redir	*redirection;
	struct s_cmd	*next;
	struct s_cmd	*prev;
	struct s_data	*data_p;
}	t_cmd;

//int start is used in expander for norm
typedef struct s_data
{
	struct s_token	*token;
	struct s_var	*var;
	struct s_cmd	*cmd;
	struct s_redir	*redir;
	char			*built_in[8];
	void			(*built_in_functions[8])(struct s_data *data, t_cmd *cmd);
	int				**pipes;
	int				*child_pids;
	int				size;
	int				exit_code;
	char			*pwd;
	char			*old_pwd;
	int				start;
}	t_data;

//main
int				find_end_word(char *str, int i);
void			builtins_init(t_data *data);
void			print_cmd_list(t_cmd *cmd_list); ////////test
void			printtoken(t_data *data); // test
void			miniloop(t_data *data);
void			free_data(t_data *data, int exit_code);
void			free_pipex(t_data *data);

//utils1
int				ft_strncmp(const char *s1, const char *s2, size_t n);
char			*ft_strjoin(char *s1, char const *s2);
int				ft_var_lstsize(t_var *var);
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
void			ft_putstr_fd(char *str, int fd);
void			*ft_memset(void *s, int c, size_t n);

//utils4
int				ft_nbrlen(long n);
char			*ft_itoa(int n);
int				ft_cmd_lstsize(t_cmd *cmd);
char			*ft_strcat(char *s1, char *s2);
int				tab_size(char **tab);
char			*ft_strcpy(char *dest, char *src);
void			read_pipe_in(t_cmd *cmd);

//parsing0
int				is_pipe(char c);
int				is_whitespace(char c);
int				is_separator(char c);
int				is_word(char *str, int i, int in_quotes);
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
int				is_strnum(char *str);

//split
void			free_mem(char **tofree);
int				ft_countwords(char const *str, char c);
char			*ft_strduplicate(char const *str, char c);
int				todup(char **dest, char const *s, char c);
char			**ft_split(char const *s, char c);

////env1
int				ft_env_to_lst(t_data *data, char **env);
char			*get_name(char *str);
char			*get_value(char *str);
struct s_var	*init_env_var(char *name, char *value, int i);
void			add_var_list(t_data *data, struct s_var *var);

//env2
char			**var_list_to_tab(t_var *var);
void			set_exit_code(t_data *data, int exit_code);
void			get_pwd(t_data *data);
void			set_pwd(t_data *data, char *pwd, char *oldpwd);
void			set_pwd2(t_data *data, char *pwd, char *oldpwd);

//is_cmd
int				is_cmd(char *str, t_data *data);
int				test_path(char *str, char **bins);
int				test_absolute_path(char *str);
char			**get_binarys(t_data *data);
char			*get_path(t_var *var);

//quotes_double
char			*ft_strjoin_char(char *str, char c);
char			*ft_substr_quotes(char *str, int i, t_data *data);

//built_in
void			env_function(t_data *data, t_cmd *cmd);
void			pwd_function(t_data *data, t_cmd *cmd);
void			export_function(t_data *data, t_cmd *cmd);

// ft_cd
void			cd_function(t_data *data, t_cmd *cmd);

//echo
void			echo_function(t_data *data, t_cmd *cmd);
int				is_n_option(char *str);

//unset
void			unset_function(t_data *data, t_cmd *cmd);
void			unset_var(t_data *data, char *str);

// export
char			*get_value_export(char *str);
void			export_var_with_value(char **args, t_data *data, int *i);
void			export_without_value(char **args, t_data *data, int *i);
int				is_env_var(char *str, t_var *var);
void			ft_print_export(t_data *data, t_cmd *cmd);
void			export_error(t_data *data, char *value);
void			update_var(t_var *var, char *new_value);

//exit
void			exit_function(t_data *data, t_cmd *cmd);
long long int	ft_atolli(const char *str);
void			ft_free_and_exit(t_data *data, int exit_code);

//lexer
int				lexer(char *line, struct s_data *data);
void			lexer_redir(char **line, int *start, int *end, t_data *data);
int				find_end_quote(char *str, int i);
void			lexer_word(char **line, int *start, int *end, t_data *data);
void			lexer_quotes(char **line, int *start, int *end, t_data *data);
void			lexer_var(char **line, int *start, int *end, t_data *data);

//parser1
void			parser(t_data *data);
void			parser_helper(t_token **current, t_cmd **cmd, int *index);
void			add_redirection(t_cmd *cmd, t_token *token);
void			append_cmd(t_cmd **cmd_list, t_cmd *new_cmd);
void			*assign_builtin_pointer(t_data *data, char *str);
void			check_redir_syntax2(t_data *data, t_cmd *cmd);

//parser2
void			init_cmd(t_cmd **cmd, t_token *token, t_data *data);
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
void			delete_quotes(t_data *data);

//expander2
char			*change_var(t_data *data, char *var_name);
void			check_for_var2(t_data *data, char **dest, int *i, char *str);
void			check_for_var(t_data *data, char *str, t_cmd *current, int j);
void			expand_var_quotes(t_data *data);
int				find_end_var_lexer(char *str, int i);

//pipex_utils
bool			allocate_pids(t_data *data);
void			handle_exitcode(t_data *data, int status);
void			ft_redirect(t_cmd *cmd, int input, int output);
bool			ft_waitpid(t_data *data);
bool			ft_close_all_fds(t_data *data);

//pipex
int				is_simple_command(t_cmd *cmd);
int   			executor(t_data *data);
// bool			ft_create_pipes(t_cmd *cmd, int pipes_fd[2]); //oeoeoeoeoe
bool			ft_create_pipes(t_data *data);
// bool			ft_execute_cmd(t_cmd *cmd);
bool			ft_execute_cmd(t_cmd *cmd, int index);
void			child(t_cmd *cmd, t_data *data);
bool            ft_execute_pipeline(t_data *data);
void			copy_pipes(t_data *shell);

//open_utils
bool			ft_open_redir(t_data *data);
bool			ft_open_redir2(t_redir *redirection);
void			ft_open_redir_output(t_redir *redir);
void			ft_open_redir_append(t_redir *redir);
void			ft_open_redir_input(t_redir *redir);
void			ft_open_redir_heredoc(t_redir *redir);
void			ft_heredoc_handler(t_data *data);//ROB
void			ft_heredoc_search(t_cmd *cmd);//ROB
void			ft_handle_heredoc_helper(t_redir *redir, int fd, char *line);//ROB

//get_next_line
char			*get_next_line(int fd);
char			*ft_strjoin_gnl(char const *s1, char const *s2);
char			*ft_strdup_gnl(char const *s);
char			*ft_strchr_gnl(char const *s, int c);

//signaux
void	heredoc_signal(int status); //dans le fichier open-utils.c
void	c_signal(int status);
void	signal_handler(int signum);


#endif
