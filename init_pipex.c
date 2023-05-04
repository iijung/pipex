/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_pipex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minjungk <minjungk@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 01:06:21 by minjungk          #+#    #+#             */
/*   Updated: 2023/05/05 01:56:40 by minjungk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static t_pipex	*_make_content(char *command, char **envp)
{
	int				argc;
	t_pipex			*rtn;
	struct s_pipex	*content;

	content = ft_calloc(1, sizeof(struct s_pipex));
	rtn = ft_lstnew(content);
	if (rtn == NULL || content == NULL)
	{
		free(content);
		ft_lstdelone(rtn, free);
		return (NULL);
	}
	content->in_fd = -1;
	content->out_fd = -1;
	content->command = command;
	content->envp = envp;
	content->argv = ft_split(command, ' ');
	argc = 0;
	while (content->argv && content->argv[argc])
		++argc;
	content->argc = argc;
	return (rtn);
}

void	free_pipex(void *param)
{
	int						argc;
	struct s_pipex *const	content = param;

	close(content->in_fd);
	close(content->out_fd);
	content->in_fd = -1;
	content->out_fd = -1;
	argc = 0;
	while (content->argv && content->argv[argc])
		free(content->argv[argc++]);
	free(content->argv);
	content->argv = NULL;
}

t_pipex	*new_pipex(int argc, char **argv, char **envp)
{
	int		i;
	t_pipex	*rtn;
	t_pipex	*tmp;

	i = 0;
	rtn = NULL;
	while (i < argc)
	{
		tmp = _make_content(argv[i], envp);
		if (tmp == NULL)
		{
			ft_lstclear(&rtn, free_pipex);
			return (NULL);
		}
		ft_lstadd_back(&rtn, tmp);
		++i;
	}
	return (rtn);
}
