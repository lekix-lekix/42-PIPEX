/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_checking.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 16:33:01 by kipouliq          #+#    #+#             */
/*   Updated: 2024/03/13 17:21:19 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../pipex.h"

char	*test_cmd_paths(char **paths, char *cmd_test)
{
	char	*path_test;
	int		s_len;
	int		i;

	i = 0;
	while (paths[++i])
	{
		s_len = ft_strlen(paths[i]) + ft_strlen(cmd_test) + 1;
		path_test = malloc(sizeof(char) * s_len);
		if (!path_test)
			return (NULL);
		ft_strlcpy(path_test, paths[i], ft_strlen(paths[i]) + 1);
		ft_strlcat(path_test, cmd_test, s_len);
		if (access(path_test, X_OK) == 0)
        {
            free(cmd_test);
			return (path_test);
        }
		free(path_test);
	}
    free(cmd_test);
	free(path_test);
	return (NULL);
}

char	*check_cmd(t_cmd *node, char *path)
{
	char	**paths;
	char	*cmd_test;
	int		i;

	i = -1;
	if (!node->cmd[0][0])
		return (NULL);
	if (access(node->cmd[0], X_OK) == 0)
	{
		cmd_test = ft_strdup(node->cmd[0]);
		if (!cmd_test)
			return (NULL);
		return (cmd_test);
	}
	paths = ft_split(path, ':');
	if (!paths)
		return (set_alloc_err(node));
	cmd_test = add_slash(node->cmd[0]);
	if (!cmd_test)
	{
		ft_free_tab((void **)paths);
		return (set_alloc_err(node));
	}
	cmd_test = test_cmd_paths(paths, cmd_test);
	ft_free_tab((void **)paths);
	if (cmd_test)
		return (cmd_test);
	free(cmd_test);
	return (NULL);
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
