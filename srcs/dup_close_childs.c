/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dup_close_childs.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 16:29:24 by kipouliq          #+#    #+#             */
/*   Updated: 2024/03/18 18:17:30 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

int	dup_close_first_child(t_data *args_env)
{
	if (dup2(args_env->infile, 0) == -1)
		return (perror_exit("dup2", args_env));
	if (dup2(args_env->pipes[0][1], 1) == -1)
		return (perror_exit("dup2", args_env));
	if (close(args_env->pipes[0][1]) == -1)
		return (perror_exit("close", args_env));
	if (close(args_env->infile) == -1)
		return (perror_exit("close", args_env));
	return (0);
}

int	dup_close_mid_child(t_data *args_env, int i)
{
	if (dup2(args_env->pipes[i - 1][0], 0) == -1)
		return (perror_exit("dup2", args_env));
	if (dup2(args_env->pipes[i][1], 1) == -1)
		return (perror_exit("dup2", args_env));
	if (close(args_env->pipes[i - 1][0]) == -1)
		return (perror_exit("close", args_env));
	if (close(args_env->pipes[i - 1][1]) == -1)
		return (perror_exit("close", args_env));
	if (close(args_env->pipes[i][1] == -1))
		return (perror_exit("close", args_env));
	return (0);
}

int	dup_close_last_child(t_data *args_env, int i)
{
	if (dup2(args_env->pipes[i - 1][0], 0) == -1)
		return (perror_exit("dup2", args_env));
	if (dup2(args_env->outfile, 1) == -1)
		return (perror_exit("dup2", args_env));
	if (close(args_env->pipes[i - 1][0]) == -1)
		return (perror_exit("close", args_env));
	if (close(args_env->pipes[i - 1][1]) == -1)
		return (perror_exit("close", args_env));
	if (close(args_env->outfile) == -1)
		return (perror_exit("close", args_env));
	return (0);
}
