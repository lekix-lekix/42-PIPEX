/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 16:21:01 by kipouliq          #+#    #+#             */
/*   Updated: 2024/03/13 17:21:05 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../pipex.h"

char	*bash_error_cat(char *filename)
{
	char	*str;
	int		filename_len;

	filename_len = ft_strlen(filename);
	str = malloc(sizeof(char) * (filename_len + 7));
	if (!str)
		return (NULL);
	ft_strlcpy(str, "bash: ", 7);
	ft_strlcat(str, filename, filename_len + 7);
	return (str);
}

int	cmd_error(char *cmd)
{
	char	*error;
	int		cmd_len;

	cmd_len = ft_strlen(cmd);
	printf("%s\n", cmd);
	error = malloc(sizeof(char) * (cmd_len + 2));
	if (!error)
		return (0);
	ft_strlcpy(error, cmd, cmd_len);
	error[cmd_len] = ':';
	error[cmd_len + 1] = '\0';
	perror(error);
	free(error);
	return (-1);
}

int	bash_file_error_exit(t_data *args_env, char *filename)
{
	char	*error;

	error = bash_error_cat(filename);
	if (!error)
		return (-1);
	perror(error);
	free(error);
	free_exit(args_env);
	return (-1);
}

void	print_cmd_error_exit(t_data *args_env, char *cmd)
{
	write(2, cmd, ft_strlen(cmd));
	write(2, ": command not found\n", 20);
	free_exit(args_env);
}
