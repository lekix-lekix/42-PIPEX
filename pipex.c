/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 14:07:27 by kipouliq          #+#    #+#             */
/*   Updated: 2024/03/01 17:03:47 by kipouliq         ###   ########.fr       */
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
	int		i;

	i = 0;
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
	while (++i < args_nb)
	{
		// printf("arg = %s\n", argv[i]);
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

int	exec_cmd_lst(t_cmd **lst)
{
	t_cmd	*current;
	int		pipe_fds[2];
    int     pid;

	current = *lst;
	while (current)
	{
        
	}
}

int	main(int argc, char **argv, char **envp)
{
	char	*path;
	int		outfile;
	t_cmd	*cmds;
	t_cmd	*current;

	// char	**cmd;
	// int		pipe_fds[2];
	// int		pid;
	// int		end_child;
	// char	read_child[200];
	// int		saved_stdin;
	if (access(argv[1], R_OK) == -1)
		return (bash_return_error(argv[1], NULL));
	if (access(argv[argc - 1], R_OK) == -1)
		outfile = open(argv[argc - 1], O_CREAT, 700); // need protection
	path = get_path(envp);
	if (!path)
		return (-1); // need free close whatever
	cmds = create_cmd_lst(argv + 2, argc - 2);
	if (!cmds)
		return (-1); // need free close whatever
	current = cmds;
	check_cmds(&cmds, path);
	if (!check_failed_alloc(&cmds))
		exec_cmd_lst(cmds);
	while (current)
	{
		if (current->cmd)
			print_tab(current->cmd);
		if (current->execve_args)
			print_tab(current->execve_args);
		current = current->next;
	}
	// path = check_cmd(path, cmd[0]);
	// if (!path)
	// {
	// 	// free : path (str), cmd (tab)
	// 	ft_printf("%s: command not found\n", cmd[0]);
	// 	free(path);
	// 	return (ft_free_tab(cmd));
	// }
	// printf("cmd %s\n", cmd[1]);
	// saved_stdin = dup(1);
	// pipe(pipe_fds);
	// pid = fork();
	// printf("pid = %d\n", pid);
	// if (pid == 0)
	// {
	// 	close(pipe_fds[0]);
	// 	dup2(pipe_fds[1], 1);
	// if (execve("/usr/bin/ls", cmd, envp) == -1)
	// printf("execve pb\n");
	// }
	// else
	// {
	// 	close(pipe_fds[1]);
	// 	waitpid(pid, &end_child, 0);
	// 	read(pipe_fds[0], read_child, 200);
	// }
	// printf("%s", read_child);
	// printf("coucocu\n");
}
