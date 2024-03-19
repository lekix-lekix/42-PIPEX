/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 14:07:27 by kipouliq          #+#    #+#             */
/*   Updated: 2024/03/19 16:53:48 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

int	init_args_env(t_data *args_env, int argc, char **argv, char **envp)
{
	args_env->argc = argc;
	args_env->argv = argv + 1;
	args_env->envp = envp;
	args_env->filename = NULL;
	args_env->infile = 0;
	args_env->outfile = 0;
	args_env->here_doc = 0;
	args_env->pipes = NULL;
	args_env->cmd_lst = NULL;
	args_env->pids = NULL;
	args_env->path = NULL;
	args_env->path = get_path(envp, args_env);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_cmd	*cmd_lst;
	t_data	args_env;

	if (argc < 5)
	{
		write(2, "bash: syntax error\n", 19);
		return (-1);
	}
	init_args_env(&args_env, argc, argv, envp);
	cmd_lst = NULL;
	args_env.cmd_lst = &cmd_lst;
	if (!ft_strncmp(argv[1], "here_doc", 8) && ft_strlen(argv[1]) == 8)
		handle_here_doc(&args_env);
	else
		create_cmd_lst(argv + 2, argc - 2, &args_env);
	check_cmds(&args_env);
	exec_cmd_lst(&args_env);
	free_cmd_lst(args_env.cmd_lst);
	if (args_env.here_doc)
    {
        unlink(args_env.filename);
		free(args_env.filename);
    }
	free(args_env.path);
}

// ! : absolute paths