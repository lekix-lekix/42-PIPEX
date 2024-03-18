/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 17:00:22 by kipouliq          #+#    #+#             */
/*   Updated: 2024/03/18 18:20:08 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../pipex.h"

void	set_tab_null(char **tab, int alloc_nb)
{
	int	i;

	i = -1;
	while (++i < alloc_nb)
		tab[i] = NULL;
}

char	*get_path(char **envp, t_data *args_env)
{
	char	*str;
	int		i;

	i = -1;
    str = NULL;
    (void) args_env;
	while (envp && envp[++i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
        {
			str = ft_strtrim(envp[i], "PATH="); // malloc ok
            if (!str)
                mem_error_exit(args_env);
        }
	}
	return (str);
}

char	**create_execve_args(t_cmd *node, char *cmd_path)
{
	char	**args;
	int		i;

	args = malloc(sizeof(char *) * 4); // malloc ok
	if (!args)
        return (NULL);
	set_tab_null(args, 3);
	args[0] = cmd_path;
	i = 0;
	while (node->cmd[++i])
	{
		args[1] = ft_strjoin_space(args[1], node->cmd[i]); // malloc ok
		if (!args[1])
		{
            free(args);
			return (NULL);
		}
	}
	args[3] = NULL;
	return (args);
}

int	wait_all_pid(int *pids, int count)
{
	int	status;
	int	i;

	i = 0;
	while (i < count)
	{
		waitpid(pids[i], &status, WUNTRACED); // to protect ?
		i++;
	}
	return (status);
}
