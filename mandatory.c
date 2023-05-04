/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mandatory.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minjungk <minjungk@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 10:17:59 by minjungk          #+#    #+#             */
/*   Updated: 2023/05/05 01:38:41 by minjungk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char *argv[], char *envp[])
{
	int				ret;
	t_pipex			*pipex;
	struct s_pipex	*content;

	if (argc < 4)
		return (EXIT_FAILURE);
	pipex = new_pipex(argc - 1 - 1, argv + 1, envp);
	content = pipex->content;
	content->infile = argv[1];
	content = ft_lstlast(pipex)->content;
	content->outfile = argv[argc - 1];
	content->outflag = O_CREAT | O_TRUNC | O_WRONLY;
	ret = run_pipex(pipex);
	ft_lstclear(&pipex, free_pipex);
	return (ret);
}
