/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minjungk <minjungk@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 09:24:27 by minjungk          #+#    #+#             */
/*   Updated: 2023/05/05 15:11:43 by minjungk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <sys/wait.h>

extern void	replace_binary(char **envp, char **cmd);

static void	ft_assert(int condition, char *file, int line)
{
	const int	save_errno = errno;

	if (condition == 0)
		return ;
	ft_putstr_fd(file, STDERR_FILENO);
	ft_putstr_fd(":", STDERR_FILENO);
	ft_putnbr_fd(line, STDERR_FILENO);
	errno = save_errno;
	perror(":");
	exit(EXIT_FAILURE);
}

static void	_redirection(struct s_pipex *content)
{
	if (content->in_fd == -1 && content->infile)
		content->in_fd = open(content->infile, O_RDONLY);
	if (content->in_fd != -1)
	{
		if (dup2(content->in_fd, STDIN_FILENO) == -1)
			ft_assert(1, __FILE__, __LINE__);
		close(content->in_fd);
		content->in_fd = -1;
	}
	if (content->outfile)
	{
		if (content->out_fd != -1)
			close(content->out_fd);
		content->out_fd = open(content->outfile, content->outflag, 0644);
	}
	if (content->out_fd != -1)
	{
		if (dup2(content->out_fd, STDOUT_FILENO) == -1)
			ft_assert(1, __FILE__, __LINE__);
		close(content->out_fd);
		content->out_fd = -1;
	}
}

static void	_exec(void *param)
{
	int						pipes[2];
	struct s_pipex *const	content = param;

	if (content == NULL)
		return ;
	ft_assert(pipe(pipes) == -1, __FILE__, __LINE__);
	content->pid = fork();
	if (content->pid == 0)
	{
		close(pipes[0]);
		ft_assert(dup2(pipes[1], STDOUT_FILENO) == -1, __FILE__, __LINE__);
		close(pipes[1]);
		_redirection(content);
		if (access(content->argv[0], F_OK | X_OK) == -1)
			replace_binary(content->envp, content->argv);
		execve(content->argv[0], content->argv, content->envp);
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
	struct s_pipex *const	content = param;

	if (content == NULL)
		return ;
	waitpid(content->pid, &content->exit_status, 0);
}

int	run_pipex(t_pipex *pipex)
{
	int				ret;
	const int		save_stdin = dup(STDIN_FILENO);
	const int		save_stdout = dup(STDOUT_FILENO);
	struct s_pipex	*content;

	ret = EXIT_FAILURE;
	if (save_stdin != -1 && save_stdout != -1)
	{
		content = ft_lstlast(pipex)->content;
		content->out_fd = save_stdout;
		{
			ft_lstiter(pipex, _exec);
			ft_lstiter(pipex, _wait);
			ret = content->exit_status;
		}
		dup2(save_stdout, STDOUT_FILENO);
		dup2(save_stdin, STDIN_FILENO);
	}
	if (save_stdin != -1)
		close(save_stdin);
	if (save_stdout != -1)
		close(save_stdout);
	return (ret);
}
