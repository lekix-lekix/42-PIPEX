/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 14:07:27 by kipouliq          #+#    #+#             */
/*   Updated: 2024/03/07 18:39:55 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./pipex.h"

int ft_free_int_tab(int **tab)
{
    int i;

    i = -1;
    while (tab[++i])
        free(tab[i]);
    free(tab);
    return (0);
}

int ft_free_char_tab(char **tab)
{
    int i;

    i = -1;
    while (tab[++i])
        free(tab[i]);
    free(tab);
    return (0);
}

int	ft_free_tab(void **tab)
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
		ft_free_tab((void **)paths);
		return (set_alloc_err(node));
	}
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
		if (access(path_test, X_OK) == 0)
		{
			free(cmd_test);
	        ft_free_tab((void **)paths);
			return (path_test);
		}
		free(path_test);
	}
	ft_free_tab((void **)paths);
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
	ft_strlcat(str, filename, filename_len + 7);
	return (str);
}

int	bash_return_error(char *filename, char **to_free)
{
	char	*error;

	if (to_free)
		ft_free_tab((void **)to_free);
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
			ft_free_tab((void **)arg);
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

	args = malloc(sizeof(char *) * 4);
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
	}
    args[3] = NULL;
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
	tab = malloc(sizeof(int *) * (size + 1));
	if (!tab)
		return (NULL);
	while (++i < size)
	{
		tab[i] = malloc(sizeof(int) * int_nb);
		if (!tab[i])
			return (NULL);
	}
    tab[i] = NULL;
	return (tab);
}

int	init_first_child(t_cmd *current, t_data *args_env, int *pipe)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		close(pipe[0]);
		if (args_env->infile == -1)
		{
			bash_return_error(args_env->argv[0], NULL);
			exit(-1);
		}
		if (!current->execve_args)
		{
			ft_printf("%s: command not found\n", current->cmd[0]);
			exit(-1);
		}
		if (dup2(args_env->infile, 0) == -1)
			return (-1);
		if (dup2(pipe[1], 1) == -1)
			return (-1);
		close(pipe[0]);
		close(pipe[1]);
		close(args_env->infile);
		if (execve(current->execve_args[0], current->execve_args,
				args_env->envp) == -1)
			return (-1);
	}
	return (pid);
}

int	init_last_child(t_cmd *current, t_data *args_env, int **pipes, int i)
{
	int	pid;
	// int	outfile;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		if (args_env->outfile == -1)
		{
			bash_return_error(args_env->argv[0], NULL);
			exit(-1);
		}
		if (!(current->execve_args))
		{
			ft_printf("%s: command not found\n", current->cmd[0]); // close pipe !
			exit(-1);
		}
		if (dup2(pipes[i - 1][0], 0) == -1)
			return (-1);
		if (dup2(args_env->outfile, 1) == -1)
			return (-1);
		close(pipes[i - 1][0]);
		close(pipes[i - 1][1]);
		if (execve(current->execve_args[0], current->execve_args,
				args_env->envp) == -1)
			return (-1);
	}
	close(pipes[i - 1][0]);
	close(pipes[i - 1][1]);
	return (pid);
}

int	init_child(t_cmd *current, t_data *args_env, int **pipes, int i)
{
	int	pid;

	if (pipe(pipes[i]) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		close(pipes[i][0]);
		if (!(current->execve_args))
		{
			ft_printf("%s: command not found\n", current->cmd[0]); // close pipe !
			exit(-1);
		}
		if (dup2(pipes[i - 1][0], 0) == -1)
			return (-1);
		if (dup2(pipes[i][1], 1) == -1)
			return (-1);
		close(pipes[i - 1][0]);
		close(pipes[i - 1][1]);
		close(pipes[i][1]);
		if (execve(current->execve_args[0], current->execve_args,
				args_env->envp) == -1)
			return (-1);
	}
	close(pipes[i - 1][0]);
	close(pipes[i - 1][1]);
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
    int     lst_size;
	int		i;

	current = *lst;
    printf("lst size = %d\n", ft_list_size(lst));
    lst_size = ft_list_size(lst);
	pipe_fds = alloc_int_tab(lst_size, 2); // pas bon, si > 2-> lst_size - 1
	if (!pipe_fds)
		return (-1);
	pids = malloc(sizeof(int) * lst_size);
	if (!pids)
		return (-1);
	i = 0;
	if (pipe(pipe_fds[0]) == -1)
		return (-1);
	while (current)
	{
		if (i == 0)
			pids[i] = init_first_child(current, args_env, pipe_fds[i]);
		else if (!current->next)
			pids[i] = init_last_child(current, args_env, pipe_fds, i);
		else
			pids[i] = init_child(current, args_env, pipe_fds, i);
		i++;
		current = current->next;
	}
	// close(pipe_fds[0][0]);
	// close(pipe_fds[0][1]);
	// close(pipe_fds[1][0]);
	// close(pipe_fds[1][1]);
	wait_all_pid(pids, i);
    ft_free_tab((void **)pipe_fds);
    free(pids);
	return (0);
}

int	handle_here_doc(t_cmd **cmd_lst, t_data *args_env)
{
    char *line;
    size_t limiter_len;

    (void) cmd_lst;
    line = NULL;
    limiter_len = ft_strlen(args_env->argv[1]);
	args_env->infile = open("./", __O_TMPFILE | O_RDWR); // to protect
    args_env->outfile = open(args_env->argv[args_env->argc - 2], O_WRONLY | O_APPEND | O_CREAT, 0777);
    while (1)
    {
        ft_printf("> ");
        line = get_next_line(0, 0);
        if (!line)
            return (-1);
        write(args_env->infile, line, ft_strlen(line));
        if (ft_strlen(line) - 1 == limiter_len && !ft_strncmp(line, args_env->argv[1], limiter_len))
            break;
    }
    get_next_line(0, 1);
    *cmd_lst = create_cmd_lst(args_env->argv + 2, 3);
    return (0);
}

int free_cmd_lst(t_cmd **lst)
{
    t_cmd   *current;
    t_cmd   *next;
    int     i;

    current = *lst;
    while (current)
    {
        i = -1;
        while (current->cmd[++i])
            free(current->cmd[i]);
        if (current->cmd)
            free(current->cmd);
        i = -1;
        while (current->execve_args[++i])
            free(current->execve_args[i]);
        if (current->execve_args)
            free(current->execve_args);
        next = current->next;
        free(current);
        current = next;
    }
    return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_cmd	*cmd_lst;
	t_data	args_env;
	char	*path;

	path = get_path(envp);
	if (!path)
		return (-1); // need free close whatever
	args_env.argc = argc;
	args_env.argv = argv + 1;
	args_env.envp = envp;
	if (argc == 6 && !ft_strncmp(argv[1], "here_doc", 8))
		handle_here_doc(&cmd_lst, &args_env);
    else
    {
		args_env.infile = open(argv[1], O_RDONLY); // need protection
        args_env.outfile = open(argv[argc - 1], O_WRONLY | O_TRUNC | O_CREAT, 0777); // need protection
        cmd_lst = create_cmd_lst(argv + 2, argc - 2);
    	if (!cmd_lst)
	    	return (-1); // need free close whatever
    }
	check_cmds(&cmd_lst, path);
	if (!check_failed_alloc(&cmd_lst))
		exec_cmd_lst(&cmd_lst, &args_env);
    close(args_env.infile);
    close(args_env.outfile);
    free_cmd_lst(&cmd_lst);
    free(path);
}


// ! : absolute paths