/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_funcs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 16:17:43 by kipouliq          #+#    #+#             */
/*   Updated: 2024/03/19 13:51:56 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

int	ft_free_char_tab(char **tab)
{
	int	i;

	i = -1;
	while (tab && tab[++i])
	{
		free(tab[i]);
		tab[i] = NULL;
	}
	if (tab)
		free(tab);
	return (0);
}

int	ft_free_tab(void **tab)
{
	int	i;

	i = -1;
	while (tab && tab[++i])
		free(tab[i]);
	free(tab);
	return (-1);
}

int	free_cmd_lst(t_cmd **lst)
{
	t_cmd	*current;
	t_cmd	*next;

	current = *lst;
	while (current)
	{
		ft_free_char_tab(current->cmd);
		ft_free_char_tab(current->execve_args);
		next = current->next;
		free(current);
		current = next;
	}
	return (0);
}

int	close_pipes(int **pipes)
{
	int	i;

	i = -1;
	while (pipes && pipes[++i])
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
	}
	return (0);
}

void	free_exit(t_data *args_env)
{
	close_pipes(args_env->pipes);
	if (args_env->cmd_lst)
		free_cmd_lst(args_env->cmd_lst);
	if (args_env->pipes)
		close_pipes(args_env->pipes);
	if (args_env->pipes)
		ft_free_tab((void **)args_env->pipes);
	if (args_env->pids)
		free(args_env->pids);
	if (args_env->path)
		free(args_env->path);
	if (args_env->filename)
		free(args_env->filename);
	exit(-1);
}
