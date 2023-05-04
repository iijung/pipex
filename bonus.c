/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minjungk <minjungk@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 19:28:30 by minjungk          #+#    #+#             */
/*   Updated: 2023/05/05 02:55:24 by minjungk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

extern int	heredoc(char *word);

int	main(int argc, char **argv, char **envp)
{
	int				ret;
	char			*heredoc_word;
	t_pipex			*pipex;
	struct s_pipex	*content;

	heredoc_word = NULL;
	if (argc > 1 && ft_strncmp(argv[1], "here_doc", 9) == 0)
		heredoc_word = argv[2];
	if (argc < 4 || (heredoc_word && argc < 6))
		return (EXIT_FAILURE);
	if (heredoc_word == NULL)
		pipex = new_pipex(argc - 1 - 1, argv + 1, envp);
	else
		pipex = new_pipex(argc - 3 - 1, argv + 3, envp);
	content = pipex->content;
	content->in_fd = heredoc(heredoc_word);
	if (heredoc_word == NULL)
		content->infile = argv[1];
	content = ft_lstlast(pipex)->content;
	content->outfile = argv[argc - 1];
	content->outflag = O_CREAT | O_TRUNC | O_WRONLY;
	ret = run_pipex(pipex);
	ft_lstclear(&pipex, free_pipex);
	return (ret);
}
