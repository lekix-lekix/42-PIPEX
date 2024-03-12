/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 15:20:22 by kipouliq          #+#    #+#             */
/*   Updated: 2024/03/12 16:22:39 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

typedef struct s_cmd
{
	char			**cmd;
    char            **execve_args;
    int             failed_alloc;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_data
{
    char            **argv;
    char            **envp;
    t_cmd           **cmd_lst;
    int             **pipes;
    char            *path;
    int             *pids;
    int             argc;
    int             infile;
    int             outfile;
}                   t_data;

# include "./42-MEGALIBFT/megalibft.h"
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

#endif