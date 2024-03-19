/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_checking.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 16:33:01 by kipouliq          #+#    #+#             */
/*   Updated: 2024/03/19 13:44:12 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

char	*test_cmd_paths(char **paths, char *cmd_test, t_cmd *node)
{
	char	*path_test;
	int		s_len;
	int		i;

	i = 0;
	while (paths[++i])
	{
		s_len = ft_strlen(paths[i]) + ft_strlen(cmd_test) + 1;
		path_test = malloc(sizeof(char) * s_len); // malloc ok
		if (!path_test)
		{
			free(cmd_test);
			return (set_alloc_err(node, paths));
		}
		ft_strlcpy(path_test, paths[i], ft_strlen(paths[i]) + 1);
		ft_strlcat(path_test, cmd_test, s_len);
		if (access(path_test, X_OK) == 0)
		{
			ft_free_tab((void **)paths);
			free(cmd_test);
			return (path_test);
		}
		free(path_test);
	}
	ft_free_tab((void **)paths);
	free(cmd_test);
	return (NULL);
}

char	*check_cmd(t_cmd *node, char *path)
{
	char	**paths;
	char	*cmd_test;

	if (access(node->cmd[0], X_OK) == 0)
	{
		cmd_test = ft_strdup(node->cmd[0]); // malloc ok
		if (!cmd_test)
			return (set_alloc_err(node, NULL));
		return (cmd_test);
	}
	if (path)
	{
		paths = ft_split(path, ':'); // malloc ok
		if (!paths)
			return (set_alloc_err(node, NULL));
		cmd_test = add_slash(node->cmd[0]); // malloc ok
		if (!cmd_test)
			return (set_alloc_err(node, paths));
		return (test_cmd_paths(paths, cmd_test, node));
	}
	return (NULL);
}

void	check_cmds(t_data *args_env)
{
	t_cmd	*current;
	char	*cmd_path;

	current = *args_env->cmd_lst;
	while (current)
	{
		cmd_path = check_cmd(current, args_env->path);
		if (!cmd_path && current->failed_alloc)
			mem_error_exit(args_env);
		else if (!cmd_path)
			current->execve_args = NULL;
		else
        {
			current->execve_args = create_execve_args(current, cmd_path); // malloc ok
            if (!current->execve_args)
            {
                free(cmd_path);
                mem_error_exit(args_env);
            }
        }
		current = current->next;
	}
}
