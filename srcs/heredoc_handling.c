/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handling.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 17:02:55 by kipouliq          #+#    #+#             */
/*   Updated: 2024/03/13 17:21:05 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../pipex.h"

char	*create_random_filename(void)
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
	return (filename);
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

t_cmd	*handle_here_doc(t_data *args_env)
{
	t_cmd	*cmd_lst;
	size_t	limiter_len;
	char	*line;

	line = NULL;
	args_env->here_doc = 1;
	limiter_len = ft_strlen(args_env->argv[1]);
	args_env->filename = create_random_filename();
		// need to check if file exists
	if (!args_env->filename)
		return (NULL); // need free
	args_env->infile = open(args_env->filename, O_RDWR | O_CREAT, 0777);
	if (args_env->infile == -1)
		bash_file_error_exit(args_env, args_env->filename);
	if (write_heredoc_file(args_env, args_env->argv[1]) == -1)
		return (NULL);
	close(args_env->infile);
	cmd_lst = create_cmd_lst(args_env->argv + 2, args_env->argc - 3);
	if (!cmd_lst)
		return (NULL); // need free
	return (cmd_lst);
}
