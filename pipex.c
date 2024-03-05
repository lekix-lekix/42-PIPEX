/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 14:07:27 by kipouliq          #+#    #+#             */
/*   Updated: 2024/03/05 18:29:36 by lekix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./pipex.h"

int	ft_free_tab(char **tab)
{
	int	i;

	i = -1;
	while (tab[++i])
		free(tab[i]);
	free(tab);
	return (-1);
}

int	ft_free_str(char *str)
{
	free(str);
	return (-1);
}

char	*add_slash(char *str)
{
	char	*cmd;
	int		str_len;

	str_len = ft_strlen(str);
	cmd = malloc(sizeof(char) * (str_len + 2));
	if (!cmd)
		return (0);
	cmd[0] = '/';
	ft_strlcpy(cmd + 1, str, str_len + 1);
	// free(str);
	return (cmd);
}

char	*set_alloc_err(t_cmd *node)
{
	node->failed_alloc = 1;
	return (NULL);
}

char	*check_cmd(t_cmd *node, char *path)
{
	char	**paths;
	char	*path_test;
	char	*cmd_test;
	int		s_len;
	int		i;

	i = -1;
	paths = ft_split(path, ':');
	if (!paths)
		return (set_alloc_err(node));
	cmd_test = add_slash(node->cmd[0]);
	if (!cmd_test)
	{
		ft_free_tab(paths);
		return (set_alloc_err(node));
	}
	// printf("node cmd[0] = %s\n", node->cmd[0]);
	while (paths[++i])
	{
		s_len = ft_strlen(paths[i]) + ft_strlen(cmd_test) + 1;
		path_test = malloc(sizeof(char) * s_len);
		if (!path_test)
		{
			set_alloc_err(node);
			return (NULL);
		}
		ft_strlcpy(path_test, paths[i], ft_strlen(paths[i]) + 1);
		ft_strlcat(path_test, cmd_test, s_len);
		// printf("path test = %s\n", path_test);
		if (access(path_test, X_OK) == 0)
		{
			free(cmd_test);
			return (path_test);
		}
		free(path_test);
	}
	ft_free_tab(paths);
	free(cmd_test);
	return (NULL);
}

char	*get_path(char **envp)
{
	char	*str;
	int		i;

	i = -1;
	while (envp[++i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
			str = ft_strtrim(envp[i], "PATH=");
	}
	return (str);
}

char	*bash_error_cat(char *filename)
{
	char	*str;
	int		filename_len;

	filename_len = ft_strlen(filename);
	str = malloc(sizeof(char) * (filename_len + 7));
	if (!str)
		return (NULL);
	ft_strlcpy(str, "bash: ", 7);
	ft_strlcat(str, filename, filename_len + 6);
	return (str);
}

int	bash_return_error(char *filename, char **to_free)
{
	char	*error;

	if (to_free)
		ft_free_tab(to_free);
	error = bash_error_cat(filename);
	if (!error)
		return (-1);
	perror(error);
	free(error);
	return (-1);
}

int	cmd_error(char *cmd)
{
	char	*error;
	int		cmd_len;

	cmd_len = ft_strlen(cmd);
	printf("%s\n", cmd);
	error = malloc(sizeof(char) * (cmd_len + 2));
	if (!error)
		return (0);
	ft_strlcpy(error, cmd, cmd_len);
	error[cmd_len] = ':';
	error[cmd_len + 1] = '\0';
	perror(error);
	free(error);
	return (-1);
}

int	tab_size(char **tab)
{
	int	i;

	i = -1;
	while (tab[i])
		i++;
	return (i);
}

t_cmd	*create_cmd_node(char **arg)
{
	t_cmd	*node;

	node = malloc(sizeof(t_cmd));
	if (!node)
		return (NULL);
	node->cmd = arg;
	node->failed_alloc = 0;
	node->execve_args = NULL;
	node->next = NULL;
	return (node);
}

void	add_cmd_node(t_cmd **lst, t_cmd *node)
{
	t_cmd	*current;

	current = *lst;
	while (current->next)
		current = current->next;
	current->next = node;
}

char	*ft_strjoin_space(char const *s1, char const *s2)
{
	size_t	s1_size;
	size_t	s2_size;
	char	*final_str;

	final_str = NULL;
	if (!s1)
	{
		final_str = ft_strdup(s2);
		if (!final_str)
			return (NULL);
		return (final_str);
	}
	s1_size = ft_strlen(s1);
	s2_size = ft_strlen(s2);
	final_str = malloc(sizeof(char) * (s1_size + s2_size) + 2);
	if (!final_str)
		return (NULL);
	ft_strlcpy(final_str, s1, s1_size + 1);
	final_str[s1_size] = ' ';
	ft_strlcpy(final_str + s1_size + 1, s2, s2_size + 1);
	free((void *)s1);
	return (final_str);
}

t_cmd	*create_cmd_lst(char **argv, int args_nb)
{
	t_cmd	*cmd_lst;
	t_cmd	*node;
	char	**arg;
	int		i;

	i = -1;
	cmd_lst = NULL;
	while (++i < args_nb - 1) // a verifier pourquoi args_nb - 1
	{
		arg = ft_split(argv[i], ' ');
		if (!arg)
			return (NULL);
		node = create_cmd_node(arg);
		if (!node)
		{
			ft_free_tab(arg);
			return (NULL);
		}
		if (!cmd_lst)
			cmd_lst = node;
		else
			add_cmd_node(&cmd_lst, node);
	}
	return (cmd_lst);
}

void	set_tab_null(char **tab, int alloc_nb)
{
	int	i;

	i = -1;
	while (++i < alloc_nb)
		tab[i] = NULL;
}

char	**create_execve_args(t_cmd *node, char *cmd_path)
{
	char	**args;
	int		i;

	args = malloc(sizeof(char *) * 3);
	if (!args)
	{
		node->failed_alloc = 1;
		return (NULL);
	}
	set_tab_null(args, 3);
	args[0] = cmd_path;
	i = 0;
	while (node->cmd[++i])
	{
		args[1] = ft_strjoin_space(args[1], node->cmd[i]);
		if (!args[1])
		{
			set_alloc_err(node);
			return (NULL);
		}
		// printf("args[1] = %s, node->cmd[%d] = %s\n",args[1], i,
		// node->cmd[i]);
	}
	return (args);
}

void	check_cmds(t_cmd **cmds_lst, char *path)
{
	t_cmd	*current;
	char	*cmd_path;

	current = *cmds_lst;
	while (current)
	{
		cmd_path = check_cmd(current, path);
		// printf("node->cmd[0] = %s\n", current->cmd[0]);
		// printf("cmd path = %s\n", cmd_path);
		if (!cmd_path && current->failed_alloc)
			return ;
		else if (!cmd_path)
			current->execve_args = NULL;
		else
			current->execve_args = create_execve_args(current, cmd_path);
		current = current->next;
	}
}

void	print_tab(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return ;
	while (tab[i])
	{
		if (tab[i])
			printf("%s\n", tab[i]);
		i++;
	}
}

int	check_failed_alloc(t_cmd **lst)
{
	t_cmd	*current;

	current = *lst;
	while (current)
	{
		if (current->failed_alloc)
			return (1);
		current = current->next;
	}
	return (0);
}

int	ft_list_size(t_cmd **lst)
{
	t_cmd	*current;
	int		i;

	i = 0;
	current = *lst;
	while (current)
	{
		i++;
		current = current->next;
	}
	return (i);
}

int	**alloc_int_tab(int size, int int_nb)
{
	int	**tab;
	int	i;

	i = -1;
	tab = malloc(sizeof(int *) * size);
	if (!tab)
		return (NULL);
	while (++i < size)
	{
		tab[i] = malloc(sizeof(int) * int_nb);
		if (!tab[i])
			return (NULL);
	}
	return (tab);
}

int	init_first_child(t_cmd *current, t_data *args_env, int *pipe)
{
	int	pid;
	int	infile;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		close(pipe[0]);
		infile = open(args_env->argv[0], O_RDONLY);
		if (infile == -1)
			return (bash_return_error(args_env->argv[0], NULL));
		if (!current->execve_args)
			return (ft_printf("%s: command not found\n", current->cmd[0]));
		if (dup2(infile, 0) == -1)
			return (-1);
		if (dup2(pipe[1], 1) == -1)
			return (-1);
		close(pipe[1]);
		if (execve(current->execve_args[0], current->execve_args,
				args_env->envp) == -1)
			return (-1);
	}
	return (pid);
}

int	init_last_child(t_cmd *current, t_data *args_env, int *pipe)
{
	int	pid;
	int	outfile;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		if (access(args_env->argv[args_env->argc], R_OK) == -1)
			outfile = open(args_env->argv[args_env->argc],
					O_WRONLY | O_APPEND | O_CREAT, 0777);
        else
            ft_printf("%s: cannot overwrite file\n", bash_error_cat(args_env->argv[args_env->argc]));
		if (!(current->execve_args))
		{
			ft_printf("%s: command not found\n", current->cmd[0]);
			return (-1);
		}
		if (dup2(pipe[0], 0) == -1)
			return (-1);
		if (dup2(outfile, 1) == -1)
			return (-1);
		close(pipe[0]);
		close(pipe[1]);
		if (execve(current->execve_args[0], current->execve_args,
				args_env->envp) == -1)
			return (-1);
	}
	return (pid);
}

int	wait_all_pid(int *pids, int count)
{
	int	status;
	int	i;

	i = -1;
	while (++i < count)
		waitpid(pids[i], &status, WUNTRACED); // to protect ?
	return (status);
}

int	exec_cmd_lst(t_cmd **lst, t_data *args_env)
{
	t_cmd	*current;
	int		**pipe_fds;
	int		*pids;
	int		i;

	current = *lst;
	pipe_fds = alloc_int_tab(ft_list_size(lst), 2);
	if (!pipe_fds)
		return (-1);
	pids = malloc(sizeof(int) * (ft_list_size(lst)));
	if (!pids)
		return (-1);
	i = 0;
	if (pipe(pipe_fds[i]) == -1)
		return (-1);
	while (current)
	{
		if (i == 0)
			pids[i] = init_first_child(current, args_env, pipe_fds[0]);
		if (!current->next)
			pids[i] = init_last_child(current, args_env, pipe_fds[0]);
		i++;
		current = current->next;
	}
	close(pipe_fds[0][0]);
	close(pipe_fds[0][1]);
	wait_all_pid(pids, i);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_cmd	*cmds;
	t_data	args_env;
	char	*path;

	// t_cmd	*current;
	path = get_path(envp);
	if (!path)
		return (-1); // need free close whatever
	cmds = create_cmd_lst(argv + 2, argc - 2);
	if (!cmds)
		return (-1); // need free close whatever
	// current = cmds;
	check_cmds(&cmds, path);
	args_env.argc = argc - 1;
	args_env.argv = argv + 1;
	args_env.envp = envp;
	// while (current)
	// {
	// 	print_tab(current->execve_args);
	// 	current = current->next;
	// }
	if (!check_failed_alloc(&cmds))
		exec_cmd_lst(&cmds, &args_env);
}
