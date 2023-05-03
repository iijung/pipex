/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minjungk <minjungk@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 19:28:30 by minjungk          #+#    #+#             */
/*   Updated: 2023/05/03 20:41:16 by minjungk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

struct s_command
{
	char	*command;
	int		argc;
	char	**argv;
	char	**envp;
	int		in_fd;
	int		out_fd;
	char	*infile;
	char	*outfile;
	int		outflag;
};

static t_list	*_new_command(char *cmd, char **envp)
{
	int					argc;
	t_list				*rtn;
	struct s_command	*command;

	command = ft_calloc(1, sizeof(struct s_command));
	rtn = ft_lstnew(command);
	if (rtn == NULL || command == NULL)
	{
		free(command);
		ft_lstdelone(rtn);
		return (NULL);
	}
	command->in_fd = -1;
	command->out_fd = -1;
	command->command = cmd;
	command->envp = envp;
	command->argv = ft_split(command, ' ');
	argc = 0;
	while (command->argv && command->argv[argc])
		++argc;
	command->argc = argc;
	return (rtn);
}

static void	_free_command(void *param)
{
	int						argc;
	struct s_command *const	child = param;

	close(child->in_fd);
	close(child->out_fd);
	child->in_fd = -1;
	child->out_fd = -1;
	argc = 0;
	while (command->argv && command->argv[argc])
		free(child->argv[argc++]);
	free(child->argv);
	child->argv = NULL;
}

static t_list	*_init(int argc, char **argv, char **envp)
{
	int		i;
	t_list	*rtn;
	t_list	*tmp;

	i = 0;
	rtn = NULL;
	while (i < argc)
	{
		tmp = _new_command(argv[i], envp);
		if (tmp == NULL)
		{
			ft_lstclear(&rtn, _free_command);
			return (NULL);
		}
		ft_lstadd_back(&rtn, tmp);
		++i;
	}
	return (rtn);
}

extern int	pipex(t_list *lst);

int	main(int argc, char **argv, char **envp)
{
	int		ret;
	char	*heredoc_word;
	t_list	*lst;

	heredoc_word = NULL;
	if (argc > 1 && ft_strncmp(argv[1], "here_doc", 9) == 0)
		heredoc_word = argv[1];
	if (argc < 4 || (heredoc_word && argc < 6))
		return (EXIT_FAILURE);
	{
		if (heredoc_word == NULL)
			lst = _init(argc - 1 - 1, argv + 1, envp);
		else
			lst = _init(argc - 3 - 1 , argc + 3, envp);
	}
	command = lst->content;
	command->in_fd = heredoc(heredoc_word);
	if (heredoc_word == NULL)
		command->infile = argv[2];
	command = ft_lstlast(lst)->content;
	command->outfile = argv[argc - 1];
	command->outflag = O_CREATE | O_TRAUC | O_WRONLY;
	ret = pipex(lst);
	ft_lstclear(&lst, _free_command);
	return (ret);
}
