/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 14:07:27 by kipouliq          #+#    #+#             */
/*   Updated: 2024/02/27 17:56:29 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./42-MEGALIBFT/megalibft.h"
#include <stdio.h>  /* needed for printf() */
#include <stdlib.h> /* needed to define exit() */
#include <unistd.h> /* needed to define getpid() */

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
	return (cmd);
}

char *check_cmd(char *path, char *cmd)
{
	char	**paths;
	char	*path_test;
    char    *cmd_test;
	int		s_len;
	int		i;

	i = -1;
	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);
	cmd_test = add_slash(cmd);
	if (!cmd)
    {
    	ft_free_tab(paths);
        return (NULL);
    }
	while (paths[++i])
	{
		s_len = ft_strlen(paths[i]) + ft_strlen(cmd_test) + 1;
		path_test = malloc(sizeof(char) * s_len);
		if (!path_test)
			return (NULL);
		ft_strlcpy(path_test, paths[i], ft_strlen(paths[i]) + 1);
		ft_strlcat(path_test, cmd_test, s_len);
        printf("path test = %s\n", path_test);
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

int	main(int argc, char **argv, char **envp)
{
	char	*path;
	char	**cmd;

	(void)argc;
	if (access(argv[1], R_OK) == -1)
		return (bash_return_error(argv[1], NULL));
	path = get_path(envp);
	if (!path)
		return (-1);
	cmd = ft_split(argv[2], ' '); // free : path
	if (!cmd)
		return (ft_free_str(path));
    path = check_cmd(path, cmd[0]);
    if (!path)
    {
	    // free : path (str), cmd (tab)
        ft_printf("%s: command not found\n", cmd[0]);
		free(path);
		return (ft_free_tab(cmd));
	}
    printf("cmd %s\n", cmd[1]);
    int saved_stdin = dup(0);
    printf("s_stdin = %d\n", saved_stdin); 
    int pid = fork();
    printf("pid = %d\n", pid);
    if (pid != 0 && execve("/usr/bin/ls", cmd, envp) == -1)
    {
        printf("execve pb\n");
    }
    printf("coucocu\n");
}
