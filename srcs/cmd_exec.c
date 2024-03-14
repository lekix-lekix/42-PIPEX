/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 16:35:11 by kipouliq          #+#    #+#             */
/*   Updated: 2024/03/14 18:03:46 by lekix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

int	init_first_child(t_cmd *current, t_data *args_env)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
		exec_first_child(current, args_env);
	return (pid);
}

int	init_mid_child(t_cmd *current, t_data *args_env, int i)
{
	int	pid;

	if (pipe(args_env->pipes[i]) == -1)
		return (-1); // to protect
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
		exec_mid_child(current, args_env, i);
	close(args_env->pipes[i - 1][0]);
	close(args_env->pipes[i - 1][1]);
	return (pid);
}

int	init_last_child(t_cmd *current, t_data *args_env, int i)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
		exec_last_child(current, args_env, i);
	close(args_env->pipes[i - 1][0]);
	close(args_env->pipes[i - 1][1]);
	return (pid);
}

int	init_all_childs(t_cmd **cmd_lst, t_data *args_env, int *pids)
{
	t_cmd	*current;
	int		i;

	i = 0;
	current = *cmd_lst;
	if (pipe(args_env->pipes[0]) == -1)
		return (-1);
	while (current)
	{
		if (i == 0)
			pids[i] = init_first_child(current, args_env);
		else if (!current->next)
			pids[i] = init_last_child(current, args_env, i);
		else
			pids[i] = init_mid_child(current, args_env, i);
		i++;
		current = current->next;
	};
	return (0);
}

int	exec_cmd_lst(t_cmd **cmd_lst, t_data *args_env)
{
	int	*pids;
	int	lst_size;

	lst_size = ft_list_size(cmd_lst);
	args_env->pipes = alloc_int_tab(lst_size, 2);
	if (!args_env->pipes)
		return (-1);
	pids = malloc(sizeof(int) * lst_size);
	if (!pids)
		return (-1);
	args_env->pids = pids;
	init_all_childs(cmd_lst, args_env, pids);
	wait_all_pid(pids, lst_size);
	close_pipes(args_env->pipes);
	ft_free_tab((void **)args_env->pipes);
	free(pids);
	return (0);
}
