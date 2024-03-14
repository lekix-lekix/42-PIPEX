/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 15:20:22 by kipouliq          #+#    #+#             */
/*   Updated: 2024/03/14 18:11:58 by lekix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "./42-MEGALIBFT/megalibft.h"
# include <errno.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_cmd
{
	char			**cmd;
	char			**execve_args;
	int				failed_alloc;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_data
{
	char			**argv;
	char			**envp;
	t_cmd			**cmd_lst;
	int				**pipes;
	char			*path;
	char			*filename;
	int				*pids;
	int				here_doc;
	int				argc;
	int				infile;
	int				outfile;
}					t_data;

int					ft_free_char_tab(char **tab);
int					ft_free_tab(void **tab);
int					free_cmd_lst(t_cmd **lst);
void				free_exit(t_data *args_env);
int					bash_file_error_exit(t_data *args_env, char *filename);
void				print_cmd_error_exit(t_data *args_env, char *cmd);
int					dup_close_first_child(t_data *args_env);
int					dup_close_mid_child(t_data *args_env, int i);
int					dup_close_last_child(t_data *args_env, int i);
void				check_cmds(t_cmd **cmds_lst, char *path);
t_cmd				*create_cmd_lst(char **argv, int args_nb);
int					ft_list_size(t_cmd **lst);
char				*get_path(char **envp);
t_cmd				*handle_here_doc(t_data *args_env);
int					check_failed_alloc(t_cmd **lst);
int					**alloc_int_tab(int size, int int_nb);
char				**copy_arg(char *argv);
int					str_is_only_spaces(char *str);
char				*set_alloc_err(t_cmd *node);
char				*ft_strjoin_space(char const *s1, char const *s2);
char				**create_null_arg(void);
void				check_cmds(t_cmd **cmds_lst, char *path);
char				*add_slash(char *str);
char				**create_execve_args(t_cmd *node, char *cmd_path);
int					exec_cmd_lst(t_cmd **cmd_lst, t_data *args_env);
int					wait_all_pid(int *pids, int count);
int					exec_first_child(t_cmd *current, t_data *args_env);
int					dup_close_only_child(t_data *args_env);
int					exec_mid_child(t_cmd *current, t_data *args_env, int i);
int					exec_last_child(t_cmd *current, t_data *args_env, int i);
int					close_pipes(int **pipes);

#endif