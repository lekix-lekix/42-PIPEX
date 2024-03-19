/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handling.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 17:02:55 by kipouliq          #+#    #+#             */
/*   Updated: 2024/03/19 16:13:44 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

void    read_into_filename(t_data *args_env, int fd, char *buf, char *filename)
{
    int i;

    i = 0;
    ft_strlcpy(filename, "/tmp/", 6);
	i = -1;
	while (i < 4)
	{
		if (read(fd, buf + i, 1) == -1)
            perror_exit("read", args_env);
		if (ft_isalnum(buf[i]))
			i++;
	}
	buf[i] = '\0';
    ft_strlcat(filename, buf, 15);
}

char	*get_random_filename(t_data *args_env)
{
	char	*filename;
	char	buf[5];
	int		fd;

	filename = malloc(sizeof(char) * 16); // ok 
	if (!filename)
		mem_error_exit(args_env);
	fd = open("/dev/urandom", O_RDONLY); // ok
	if (fd == -1)
    {
        free(filename);
		perror_exit("open", args_env);
    }
	ft_strlcpy(filename, "/tmp/", 6);
	read_into_filename(args_env, fd, buf, filename);
	close(fd);
	return (filename);
}

char	*create_random_filename(t_data *args_env)
{
	char	*filename;

	filename = get_random_filename(args_env);
	if (!filename)
		mem_error_exit(args_env);
	if (access(filename, F_OK) == -1)
		return (filename);
	else
    {
        free(filename);
		return (create_random_filename(args_env));
    }
}

int	write_heredoc_file(t_data *args_env, char *limiter)
{
	size_t	limiter_len;
	char	*line;

	line = NULL;
	limiter_len = ft_strlen(limiter);
	while (1)
	{
		ft_printf("> ");
		line = get_next_line(0, 0);
		if (!line)
			free_exit(args_env);
		if (ft_strlen(line) - 1 == limiter_len && !ft_strncmp(line, limiter,
				limiter_len))
			break ;
		if (write(args_env->infile, line, ft_strlen(line)) == -1)
        {
            perror("write");
            free(line);
            free_exit(args_env);
        }
		free(line);
	}
	get_next_line(0, 1);
	free(line);
	return (0);
}

int handle_here_doc(t_data *args_env)
{
    if (args_env->argc < 6)
    {
        write(2, "bash: syntax error\n", 19);
        return (-1);
    }    
	args_env->here_doc = 1;
	args_env->filename = create_random_filename(args_env);
	if (!args_env->filename)
		return (mem_error_exit(args_env)); // need free
	args_env->infile = open(args_env->filename, O_RDWR | O_CREAT, 0777);
	if (args_env->infile == -1)
		bash_file_error_exit(args_env, args_env->filename);
	if (write_heredoc_file(args_env, args_env->argv[1]) == -1)
		return (-1);
	close(args_env->infile);
	create_cmd_lst(args_env->argv + 2, args_env->argc - 3, args_env);
	return (0);
}
