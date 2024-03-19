/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 16:35:11 by kipouliq          #+#    #+#             */
/*   Updated: 2024/03/19 15:48:48 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

int	init_first_child(t_cmd *current, t_data *args_env)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		return (perror_exit("fork", args_env));
	if (pid == 0)
		exec_first_child(current, args_env);
	return (pid);
}

int	init_mid_child(t_cmd *current, t_data *args_env, int i)
{
	int	pid;

	if (pipe(args_env->pipes[i]) == -1)
		return (perror_exit("pipe", args_env)); // ok
	pid = fork();
	if (pid == -1)
		return (perror_exit("fork", args_env)); // ok
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
		return (perror_exit("fork", args_env));
	if (pid == 0)
		exec_last_child(current, args_env, i);
	close(args_env->pipes[i - 1][0]);
	close(args_env->pipes[i - 1][1]);
	return (pid);
}

int	init_all_childs(t_data *args_env)
{
	t_cmd	*current;
	int		i;

	i = 0;
	current = *args_env->cmd_lst;
	if (pipe(args_env->pipes[0]) == -1)
		perror_exit("pipe", args_env);
	while (current)
	{
		if (i == 0)
			args_env->pids[i] = init_first_child(current, args_env);
		else if (!current->next)
			args_env->pids[i] = init_last_child(current, args_env, i);
		else
			args_env->pids[i] = init_mid_child(current, args_env, i);
		i++;
		current = current->next;
	};
	return (0);
}

int	exec_cmd_lst(t_data *args_env)
{
	int	*pids;
	int	lst_size;

	lst_size = ft_list_size(args_env->cmd_lst);
	args_env->pipes = alloc_int_tab(lst_size, 2); // malloc ok
	if (!args_env->pipes)
		mem_error_exit(args_env);
	pids = malloc(sizeof(int) * lst_size); // malloc ok
	if (!pids)
		mem_error_exit(args_env);
	args_env->pids = pids;
	init_all_childs(args_env);
	wait_all_pid(pids, lst_size);
	close_pipes(args_env->pipes);
	ft_free_tab((void **)args_env->pipes);
	free(pids);
	return (0);
}
