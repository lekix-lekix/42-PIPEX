/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dup_close_childs.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 16:29:24 by kipouliq          #+#    #+#             */
/*   Updated: 2024/03/14 18:23:06 by lekix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

int	dup_close_only_child(t_data *args_env)
{
	if (args_env->here_doc)
		args_env->outfile = open(args_env->argv[args_env->argc - 2],
				O_WRONLY | O_APPEND | O_CREAT, 0777);
	else
		args_env->outfile = open(args_env->argv[args_env->argc - 2],
				O_WRONLY | O_TRUNC | O_CREAT, 0777);
	if (args_env->outfile == -1)
		bash_file_error_exit(args_env, args_env->argv[args_env->argc - 2]);
	if (dup2(args_env->infile, 0) == -1)
		return (-1);
	if (dup2(args_env->outfile, 1) == -1)
		return (-1);
	close(args_env->pipes[0][0]);
	close(args_env->pipes[0][1]);
	close(args_env->infile);
	close(args_env->outfile);
	return (0);
}

int	dup_close_first_child(t_data *args_env)
{
	if (dup2(args_env->infile, 0) == -1)
		return (-1);
	if (dup2(args_env->pipes[0][1], 1) == -1)
		return (-1);
	close(args_env->pipes[0][0]);
	close(args_env->pipes[0][1]);
	close(args_env->infile);
	return (0);
}

int	dup_close_mid_child(t_data *args_env, int i)
{
	if (dup2(args_env->pipes[i - 1][0], 0) == -1)
		return (-1);
	if (dup2(args_env->pipes[i][1], 1) == -1)
		return (-1);
	close(args_env->pipes[i - 1][0]);
	close(args_env->pipes[i - 1][1]);
	close(args_env->pipes[i][1]);
	return (0);
}

int	dup_close_last_child(t_data *args_env, int i)
{
	if (dup2(args_env->pipes[i - 1][0], 0) == -1)
		return (-1);
	if (dup2(args_env->outfile, 1) == -1)
		return (-1);
	close(args_env->pipes[i - 1][0]);
	close(args_env->pipes[i - 1][1]);
	close(args_env->outfile);
	return (0);
}
