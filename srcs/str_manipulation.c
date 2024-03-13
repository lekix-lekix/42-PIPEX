/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_manipulation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 16:57:28 by kipouliq          #+#    #+#             */
/*   Updated: 2024/03/13 17:21:05 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

char	*add_slash(char *str)
{
	char	*cmd;
	int		str_len;

	str_len = ft_strlen(str);
	cmd = malloc(sizeof(char) * (str_len + 2));
	if (!cmd)
		return (0);
	cmd[0] = '/';
	ft_strlcpy(cmd + 1, str, str_len + 1);
	return (cmd);
}

char	*ft_strjoin_space(char const *s1, char const *s2)
{
	size_t	s1_size;
	size_t	s2_size;
	char	*final_str;

	final_str = NULL;
	if (!s1)
	{
		final_str = ft_strdup(s2);
		if (!final_str)
			return (NULL);
		return (final_str);
	}
	s1_size = ft_strlen(s1);
	s2_size = ft_strlen(s2);
	final_str = malloc(sizeof(char) * (s1_size + s2_size) + 2);
	if (!final_str)
		return (NULL);
	ft_strlcpy(final_str, s1, s1_size + 1);
	final_str[s1_size] = ' ';
	ft_strlcpy(final_str + s1_size + 1, s2, s2_size + 1);
	free((void *)s1);
	return (final_str);
}

int	str_is_only_spaces(char *str)
{
	int	i;

	i = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (!str[i])
		return (1);
	return (0);
}

char	**create_null_arg(void)
{
	char	**arg;

	arg = malloc(sizeof(char *) * 2);
	if (!arg)
		return (NULL);
	arg[0] = malloc(sizeof(char));
	if (!arg[0])
		return (NULL);
	arg[1] = NULL;
	arg[0][0] = '\0';
	return (arg);
}

char	**copy_arg(char *argv)
{
	char	**arg;

	arg = malloc(sizeof(char *) * 2);
	if (!arg)
		return (NULL);
	arg[0] = ft_strdup(argv);
	if (!arg[0])
		return (NULL);
	arg[1] = NULL;
	return (arg);
}
