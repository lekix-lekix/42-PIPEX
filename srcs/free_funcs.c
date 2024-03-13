/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_funcs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 16:17:43 by kipouliq          #+#    #+#             */
/*   Updated: 2024/03/13 17:21:05 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../pipex.h"

int	ft_free_char_tab(char **tab)
{
	int	i;

	i = -1;
	while (tab && tab[++i])
	{
		free(tab[i]);
		tab[i] = NULL;
	}
	if (tab)
		free(tab);
	return (0);
}

int	ft_free_tab(void **tab)
{
	int	i;

	i = -1;
	while (tab[++i])
		free(tab[i]);
	free(tab);
	return (-1);
}

int	free_cmd_lst(t_cmd **lst)
{
	t_cmd	*current;
	t_cmd	*next;

	current = *lst;
	while (current)
	{
		ft_free_char_tab(current->cmd);
		ft_free_char_tab(current->execve_args);
		next = current->next;
		free(current);
		current = next;
	}
	return (0);
}

void	free_exit(t_data *args_env)
{
	free_cmd_lst(args_env->cmd_lst);
	ft_free_tab((void **)args_env->pipes);
	free(args_env->pids);
	free(args_env->path);
	exit(-1);
}
