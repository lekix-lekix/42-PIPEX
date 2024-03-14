/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   childs_cmd_exec.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 12:01:56 by lekix             #+#    #+#             */
/*   Updated: 2024/03/14 18:37:19 by lekix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

int	exec_first_child(t_cmd *current, t_data *args_env)
{
	close(args_env->pipes[0][0]);
	if (args_env->here_doc)
		args_env->infile = open(args_env->filename, O_RDONLY);
	else
		args_env->infile = open(args_env->argv[0], O_RDONLY);
	if (args_env->infile == -1)
		bash_file_error_exit(args_env, args_env->argv[0]);
	if (!current->execve_args)
		print_cmd_error_exit(args_env, current->cmd[0]);
	if (ft_list_size(args_env->cmd_lst) == 1)
	{
		if (dup_close_only_child(args_env) == -1)
			return (-1);
	}
    else
	    if (dup_close_first_child(args_env) == -1)
		    return (-1);
	if (execve(current->execve_args[0], current->execve_args, args_env->envp) ==
		-1)
		return (-1);
	return (0);
}

int	exec_mid_child(t_cmd *current, t_data *args_env, int i)
{
	close(args_env->pipes[i][0]);
	if (!(current->execve_args))
		print_cmd_error_exit(args_env, current->cmd[0]);
	dup_close_mid_child(args_env, i);
	if (execve(current->execve_args[0], current->execve_args, args_env->envp) ==
		-1)
		return (-1);
	return (0);
}

int	exec_last_child(t_cmd *current, t_data *args_env, int i)
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
	if (execve(current->execve_args[0], current->execve_args, args_env->envp) ==
		-1)
		return (-1);
	return (0);
}
