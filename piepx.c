/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piepx.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minjungk <minjungk@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 09:24:27 by minjungk          #+#    #+#             */
/*   Updated: 2023/05/03 14:29:06 by minjungk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	_redirection(struct s_command *child)
{
	if (child->in_fd == -1 && child->infile)
		child->in_fd = open(child->infile, O_RDONLY);
	if (child->in_fd != -1)
	{
		ft_assert(dup2(child->in_fd, STDIN_FILENO == -1, __FILE__, __LINE__);
		close(child->in_fd);
		child->in_fd = -1;
	}
	if (child->out_fd == -1 && child->out_fd)
		child->out_fd = open(child->outfile, child->outflag, 0644);
	if (child->out_fd != -1)
	{
		ft_assert(dup2(fd, STDOUT_FILENO == -1, __FILE__, __LINE__);
		close(child->out_fd);
		child->out_fd = -1;
	}
}

static void	_exec(void *param)
{
	int						pipes[2];
	struct s_command *const	child = param;

	if (child == NULL)
		return ;
	ft_assert(pipe(pipes) == -1, __FILE__, __LINE__);
	if (fork() == 0)
	{
		close(pipes[0]);
		ft_assert(dup2(pipes[1], STDOUT_FILENO) == -1, __FILE__, __LINE__);
		close(pipes[1]);
		_redirection(child);
		execve(child->argv[0], child->argv, child->environ);
	}
	else
	{
		close(pipes[1]);
		ft_assert(dup2(pipes[0], STDIN_FILENO) == -1, __FILE__, __LINE__);
		close(pipes[0]);
	}
}

static void	_wait(void *param)
{
	struct s_command *const	child = param;

	if (child == NULL)
		return ;
	waitpid(-1, NULL, 0);
}

int	pipex(t_list *command)
{
	int					ret;
	const int			save_stdin = dup(STDIN_FILENO);
	const int			save_stdout = dup(STDOUT_FILENO);
	struct s_command	*command;
	
	ret = EXIT_FAILURE;
	if (save_stdin != -1 && save_stdout != -1)
	{
		command = ft_lstlast(command)->content;
		command->output_fd = save_stdout;
		{
			ret = EXIT_SUCCESS;
			ft_lstiter(command, _exec);
			ft_lstiter(command, _wait);
		}
		dup2(save_stdout, STDOUT_FILENO);
		dup2(save_stdin, STDIN_FILENO);
	}
	close(save_stdin);
	close(save_stdout);
	return (ret);
}
