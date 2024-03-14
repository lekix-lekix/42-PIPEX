/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_funcs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 16:53:04 by kipouliq          #+#    #+#             */
/*   Updated: 2024/03/14 13:40:52 by lekix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../pipex.h"

int	ft_list_size(t_cmd **lst)
{
	t_cmd	*current;
	int		i;

	i = 0;
	current = *lst;
	while (current)
	{
		i++;
		current = current->next;
	}
	return (i);
}

t_cmd	*create_cmd_node(char **arg)
{
	t_cmd	*node;

	node = malloc(sizeof(t_cmd));
	if (!node)
		return (NULL);
	node->cmd = arg;
	node->failed_alloc = 0;
	node->execve_args = NULL;
	node->next = NULL;
	return (node);
}

void	add_cmd_node(t_cmd **lst, t_cmd *node)
{
	t_cmd	*current;

	current = *lst;
	while (current->next)
		current = current->next;
	current->next = node;
}

char    **check_arg(char *argv)
{
    char **arg;

    if (!ft_strlen(argv))
        arg = create_null_arg();
    else if (str_is_only_spaces(argv))
        arg = copy_arg(argv);
    else
        arg = ft_split(argv, ' ');
    if (!arg)
        return (NULL);
    return (arg);
}

t_cmd	*create_cmd_lst(char **argv, int args_nb)
{
	t_cmd	*cmd_lst;
	t_cmd	*node;
	char	**arg;
	int		i;

	i = -1;
	cmd_lst = NULL;
	while (++i < args_nb - 1)
	{
        arg = check_arg(argv[i]);
        if (!arg)
            return (NULL);
		node = create_cmd_node(arg);
		if (!node)
		{
			ft_free_tab((void **)arg);
			return (NULL);
		}
		if (!cmd_lst)
			cmd_lst = node;
		else
			add_cmd_node(&cmd_lst, node);
	}
	return (cmd_lst);
}
