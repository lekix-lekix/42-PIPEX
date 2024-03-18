/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handling.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 17:02:55 by kipouliq          #+#    #+#             */
/*   Updated: 2024/03/18 11:42:06 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

char	*get_random_filename(void)
{
	char	*filename;
	char	buf[5];
	int		fd;
	int		i;

	filename = malloc(sizeof(char) * 16);
	if (!filename)
		return (NULL);
	fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1)
		return (NULL);
	ft_strlcpy(filename, "/tmp/", 6);
	i = 0;
	while (i < 4)
	{
		read(fd, buf + i, 1);
		if (ft_isalnum(buf[i]))
			i++;
	}
	buf[i] = '\0';
	ft_strlcat(filename, buf, 15);
	close(fd);
	return (filename);
}

char	*create_random_filename(void)
{
	char	*filename;

	filename = get_random_filename();
	if (!filename)
		return (NULL);
	if (access(filename, F_OK) == -1)
		return (filename);
	else
		return (create_random_filename());
}

int	write_heredoc_file(t_data *args_env, char *limiter)
{
	size_t	limiter_len;
	char	*line;

	line = NULL;
	limiter_len = ft_strlen(limiter);
	while (1) // add EOF support
	{
		ft_printf("> ");
		line = get_next_line(0, 0);
		if (!line)
			return (-1);
		if (ft_strlen(line) - 1 == limiter_len && !ft_strncmp(line, limiter,
				limiter_len))
			break ;
		write(args_env->infile, line, ft_strlen(line));
		free(line);
	}
	get_next_line(0, 1);
	free(line);
	return (0);
}

int handle_here_doc(t_data *args_env)
{
	args_env->here_doc = 1;
	args_env->filename = create_random_filename();
	if (!args_env->filename)
		return (-1); // need free
	args_env->infile = open(args_env->filename, O_RDWR | O_CREAT, 0777);
	if (args_env->infile == -1)
		bash_file_error_exit(args_env, args_env->filename);
	if (write_heredoc_file(args_env, args_env->argv[1]) == -1)
		return (-1);
	close(args_env->infile);
	create_cmd_lst(args_env->argv + 2, args_env->argc - 3, args_env);
	if (!args_env->cmd_lst)
		return (-1); // need free
	return (0);
}
