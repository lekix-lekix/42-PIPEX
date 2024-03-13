/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 16:35:11 by kipouliq          #+#    #+#             */
/*   Updated: 2024/03/13 17:21:05 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../pipex.h"

int	init_first_child(t_cmd *current, t_data *args_env)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		close(args_env->pipes[0][0]);
		if (args_env->here_doc)
			args_env->infile = open(args_env->filename, O_RDONLY);
		else
			args_env->infile = open(args_env->argv[0], O_RDONLY);
		// need protection
		if (args_env->infile == -1)
			bash_file_error_exit(args_env, current->cmd[0]);
		if (!current->execve_args)
			print_cmd_error_exit(args_env, current->cmd[0]);
		if (dup_close_first_child(args_env))
			return (-1);
		if (execve(current->execve_args[0], current->execve_args,
				args_env->envp) == -1)
			return (-1);
	}
	return (pid);
}

int	init_child(t_cmd *current, t_data *args_env, int i)
{
	int	pid;

	if (pipe(args_env->pipes[i]) == -1)
		return (-1); // to protect
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		close(args_env->pipes[i][0]);
		if (!(current->execve_args))
			print_cmd_error_exit(args_env, current->cmd[0]);
		dup_close_mid_child(args_env, i);
		if (execve(current->execve_args[0], current->execve_args,
				args_env->envp) == -1)
			return (-1);
	}
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
	{
		if (args_env->here_doc)
			args_env->outfile = open(args_env->argv[args_env->argc - 2],
					O_WRONLY | O_APPEND | O_CREAT, 0777);
		else
			args_env->outfile = open(args_env->argv[args_env->argc - 2],
					O_WRONLY | O_TRUNC | O_CREAT, 0777);
		if (args_env->outfile == -1)
			bash_file_error_exit(args_env, args_env->argv[args_env->argc - 2]);
		if (!(current->execve_args))
			print_cmd_error_exit(args_env, current->cmd[0]);
		if (dup_close_last_child(args_env, i))
			return (-1); // need free functions
		if (execve(current->execve_args[0], current->execve_args,
				args_env->envp) == -1)
			return (-1);
	}
	close(args_env->pipes[i - 1][0]);
	close(args_env->pipes[i - 1][1]);
	return (pid);
}

int	exec_cmd_lst(t_cmd **lst, t_data *args_env)
{
	t_cmd	*current;
	int		*pids;
	int		lst_size;
	int		i;

	current = *lst;
	// printf("lst size = %d\n", ft_list_size(lst));
	lst_size = ft_list_size(lst);
	args_env->pipes = alloc_int_tab(lst_size, 2);
	if (!args_env->pipes)
		return (-1);
	pids = malloc(sizeof(int) * lst_size);
	if (!pids)
		return (-1);
	args_env->pids = pids;
	i = 0;
	if (pipe(args_env->pipes[0]) == -1)
		return (-1);
	while (current)
	{
		if (i == 0)
			pids[i] = init_first_child(current, args_env);
		else if (!current->next)
			pids[i] = init_last_child(current, args_env, i);
		else
			pids[i] = init_child(current, args_env, i);
		i++;
		current = current->next;
	}
	// close(pipe_fds[0][0]);
	// close(pipe_fds[0][1]);
	// close(pipe_fds[1][0]);
	// close(pipe_fds[1][1]);
	wait_all_pid(pids, ft_list_size(lst));
	ft_free_tab((void **)args_env->pipes);
	free(pids);
	return (0);
}
