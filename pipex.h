/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minjungk <minjungk@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 22:34:32 by minjungk          #+#    #+#             */
/*   Updated: 2023/05/05 15:01:16 by minjungk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include <stdio.h>
# include "fcntl.h"
# include "libft.h"

typedef t_list	t_pipex;

struct s_pipex
{
	char	*command;
	pid_t	pid;
	int		argc;
	char	**argv;
	char	**envp;
	int		exit_status;
	int		in_fd;
	int		out_fd;
	char	*infile;
	char	*outfile;
	int		outflag;
};

extern void		free_pipex(void *param);
extern int		run_pipex(t_pipex *pipex);
extern t_pipex	*new_pipex(int argc, char **argv, char **envp);

#endif
