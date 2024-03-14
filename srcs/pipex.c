/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 14:07:27 by kipouliq          #+#    #+#             */
/*   Updated: 2024/03/14 14:27:58 by lekix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

int	init_args_env(t_data *args_env, int argc, char **argv, char **envp)
{
	char	*path;

	path = get_path(envp);
	if (!path)
		return (-1); // need free close whatever
	args_env->path = path;
	args_env->argc = argc;
	args_env->argv = argv + 1;
	args_env->envp = envp;
	args_env->here_doc = 0;
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_cmd	*cmd_lst;
	t_data	args_env;

	if (init_args_env(&args_env, argc, argv, envp) == -1)
		return (-1);
	if (!ft_strncmp(argv[1], "here_doc", 8)) // need more than strncmp ?
		cmd_lst = handle_here_doc(&args_env);
	else
	{
		cmd_lst = create_cmd_lst(argv + 2, argc - 2);
		if (!cmd_lst)
			return (-1); // need free close whatever
	}
	args_env.cmd_lst = &cmd_lst;
	check_cmds(&cmd_lst, args_env.path);
	if (!check_failed_alloc(&cmd_lst))
		exec_cmd_lst(&cmd_lst, &args_env);
	free_cmd_lst(&cmd_lst);
	free(args_env.path);
}

// ! : absolute paths