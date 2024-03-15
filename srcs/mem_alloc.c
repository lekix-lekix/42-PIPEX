/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_alloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 17:01:34 by kipouliq          #+#    #+#             */
/*   Updated: 2024/03/15 14:01:16 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../pipex.h"

int	**alloc_int_tab(int size, int int_nb)
{
	int	**tab;
	int	i;
    int j;

	i = -1;
	tab = malloc(sizeof(int *) * (size + 1));
	if (!tab)
		return (NULL);
	while (++i < size)
	{
		tab[i] = malloc(sizeof(int) * int_nb);
		if (!tab[i])
			return (NULL);
        j = -1;
        while (++j < int_nb)
            tab[i][j] = 0;
	}
	tab[i] = NULL;
	return (tab);
}

char	*set_alloc_err(t_cmd *node)
{
	node->failed_alloc = 1;
	return (NULL);
}

int	check_failed_alloc(t_cmd **lst)
{
	t_cmd	*current;

	current = *lst;
	while (current)
	{
		if (current->failed_alloc)
			return (1);
		current = current->next;
	}
	return (0);
}
