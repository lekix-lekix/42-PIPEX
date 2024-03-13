/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 14:07:27 by kipouliq          #+#    #+#             */
/*   Updated: 2024/03/13 17:21:05 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_cmd	*cmd_lst;
	t_data	args_env;
	char	*path;

	path = get_path(envp);
	if (!path)
		return (-1); // need free close whatever
	args_env.path = path;
	args_env.argc = argc;
	args_env.argv = argv + 1;
	args_env.envp = envp;
	args_env.here_doc = 0;
	if (!ft_strncmp(argv[1], "here_doc", 8)) // need more than strncmp ?
		cmd_lst = handle_here_doc(&args_env);
	else
	{
		cmd_lst = create_cmd_lst(argv + 2, argc - 2);
		if (!cmd_lst)
		{
			printf("!cmd lst");
			return (-1); // need free close whatever
		}
	}
	args_env.cmd_lst = &cmd_lst;
	check_cmds(&cmd_lst, path);
	if (!check_failed_alloc(&cmd_lst))
		exec_cmd_lst(&cmd_lst, &args_env);
	close(args_env.infile);
	close(args_env.outfile);
	free_cmd_lst(&cmd_lst);
	free(path);
}

// ! : absolute paths