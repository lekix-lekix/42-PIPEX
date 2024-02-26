/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 14:07:27 by kipouliq          #+#    #+#             */
/*   Updated: 2024/02/26 18:21:12 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./42-MEGALIBFT/megalibft.h"
#include <stdio.h>  /* needed for printf() */
#include <stdlib.h> /* needed to define exit() */
#include <unistd.h> /* needed to define getpid() */

char	*add_slash(char *str)
{
	char	*cmd;
	int		str_len;

	str_len = ft_strlen(str);
	cmd = malloc(sizeof(char) * (str_len + 2));
	if (!cmd)
		return (NULL);
	cmd[0] = '/';
	ft_strlcpy(cmd + 1, str, str_len + 1);
	free(str);
	return (cmd);
}

int	check_cmd(char *path, char *cmd)
{
	char	**paths;
    char    *path_test;
	int		i;

	i = -1;
	paths = ft_split(path, ':');
	if (!paths)
		return (-1);
    cmd = add_slash(cmd);
    while (paths[++i])
    {
        path_test = malloc(sizeof(char) * (ft_strlen(paths[i]) + ft_strlen(cmd) + 1));
        if (!path_test)
            return (-1);
        ft_strlcpy(path_test, paths[i], ft_strlen(paths[i]) + 1);
        printf("path test = %s\n", path_test);
        printf("val = %zu\n", ft_strlcat(path_test, cmd, ft_strlen(paths[i]) + ft_strlen(cmd) + 1));
        printf("path test = %s\n", path_test);
    }
	return (0);
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

int	main(int argc, char **argv, char **envp)
{
	char	*path;
	char	**cmd;

	(void)argc;
	(void)argv;
	path = get_path(envp);        // need to free
	cmd = ft_split(argv[2], ' '); // need to free
	if (!path || !cmd)
		return (-1);
	check_cmd(path, cmd[0]);
	// printf("path = '%s'\n", path);
	// printf("cmd = %s\n", cmd[0]);
}
