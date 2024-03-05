/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 15:20:22 by kipouliq          #+#    #+#             */
/*   Updated: 2024/03/05 18:16:28 by lekix            ###   ########.fr       */
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
    int             argc;
}                   t_data;

# include "./42-MEGALIBFT/megalibft.h"
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

#endif